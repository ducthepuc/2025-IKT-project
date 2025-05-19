#include "sqlite.hpp"
#include <iostream>
#include <fstream>
#include <vector>
#include <filesystem>
#include "json.hpp"
#include "httplib.h"
#include "sha256.h"

using json = nlohmann::json;
using namespace httplib;
using namespace std;

namespace app {

const string DB_FILE = "app.db";

std::string hmac_sha256(const std::string &key, const std::string &message) {
    const size_t block_size = 64;
    std::string key_block = key;

    if (key_block.size() > block_size) {
        key_block = picosha2::hash256_hex_string(key_block);
    }
    if (key_block.size() < block_size) {
        key_block.resize(block_size, '\0');
    }

    std::string o_key_pad(block_size, '\x5c');
    std::string i_key_pad(block_size, '\x36');

    for (size_t i = 0; i < block_size; ++i) {
        o_key_pad[i] ^= key_block[i];
        i_key_pad[i] ^= key_block[i];
    }

    std::string inner_data = i_key_pad + message;
    std::string inner_hash = picosha2::hash256_hex_string(inner_data);

    std::string outer_data = o_key_pad + inner_hash;
    return picosha2::hash256_hex_string(outer_data);
}

std::string createToken(int userId, int expirySecondsFromNow, const std::string& secret) {
    using namespace std::chrono;
    auto now = system_clock::now();
    int64_t expiry = duration_cast<seconds>(now.time_since_epoch()).count() + expirySecondsFromNow;

    std::ostringstream payload;
    payload << userId << ":" << expiry;

    std::string signature = hmac_sha256(secret, payload.str());
    return payload.str() + "." + signature;  
}

bool verifyToken(const std::string& token, const std::string& secret, int& userId) {
    auto pos = token.find('.');
    if (pos == std::string::npos) return false;

    std::string payload = token.substr(0, pos);
    std::string signature = token.substr(pos + 1);

    std::string expectedSig = hmac_sha256(secret, payload);
    if (signature != expectedSig) return false;

    auto sep = payload.find(':');
    if (sep == std::string::npos) return false;

    try {
        userId = std::stoi(payload.substr(0, sep));
        int64_t expiry = std::stoll(payload.substr(sep + 1));

        using namespace std::chrono;
        auto now = system_clock::now();
        int64_t nowSecs = duration_cast<seconds>(now.time_since_epoch()).count();

        if (expiry < nowSecs) return false;

    } catch (...) {
        return false;
    }

    return true;
}

void setCORS(Response &res) {
    res.set_header("Access-Control-Allow-Headers", "Content-Type, Authorization");
    res.set_header("Access-Control-Allow-Origin", "*");
    res.set_header("Access-Control-Allow-Methods", "GET, POST, OPTIONS");

}

bool requireAuth(const httplib::Request& req, httplib::Response& res, const std::string& secret, int& userIdOut) {
    setCORS(res);

    if (req.method == "OPTIONS") {
        res.status = 200;
        return false;
    }

    auto authHeader = req.get_header_value("Authorization");
    if (authHeader.empty() || authHeader.substr(0, 7) != "Bearer ") {
        res.status = 401;
        res.set_content(R"({"success":false,"message":"Missing or invalid Authorization header"})", "application/json");
        return false;
    }

    std::string token = authHeader.substr(7);
    if (!verifyToken(token, secret, userIdOut)) {
        res.status = 401;
        res.set_content(R"({"success":false,"message":"Invalid or expired token"})", "application/json");
        return false;
    }

    return true;
}

void validateTokenEndpoint(const Request& req, Response& res, const std::string& secret) {
    setCORS(res);
    if (req.method == "OPTIONS") {
        res.status = 200;
        return;
    }

    auto authHeader = req.get_header_value("Authorization");
    cout << authHeader;
    if (authHeader.empty() || authHeader.substr(0, 7) != "Bearer ") {
        res.status = 401;
        res.set_content(R"({"success":false,"message":"Missing or invalid Authorization header"})", "application/json");
        return;
    }

    std::string token = authHeader.substr(7);
    int userId = 0;
    if (!verifyToken(token, secret, userId)) {
        res.status = 401;
        res.set_content(R"({"success":false,"message":"Invalid or expired token"})", "application/json");
        return;
    }

    json response = {
        {"success", true},
        {"userId", userId}
    };
    res.set_content(response.dump(), "application/json");
}

bool saveUploadedFile(const httplib::MultipartFormData& file, const std::string& uploadDir, std::string& outFilename) {
    try {
        if (file.filename.empty()) return false;

        std::filesystem::create_directories(uploadDir);

        std::string filename = std::filesystem::path(file.filename).filename().string();

        std::string filepath = uploadDir + "/" + filename;

        std::ofstream ofs(filepath, std::ios::binary);
        ofs.write(file.content.data(), file.content.size());
        ofs.close();

        outFilename = filename;
        return true;
    } catch (...) {
        return false;
    }
}

void initializeDatabase(sqlite::Connection &db) {
    db.Statement(R"(
        CREATE TABLE IF NOT EXISTS users (
            id INTEGER PRIMARY KEY AUTOINCREMENT,
            name TEXT NOT NULL,
            email TEXT NOT NULL UNIQUE,
            password TEXT NOT NULL,
            admin BOOLEAN DEFAULT 0
        )
    )");

    db.Statement(R"(
        CREATE TABLE IF NOT EXISTS celebrity_cats (
            id INTEGER PRIMARY KEY AUTOINCREMENT,
            name TEXT NOT NULL,
            price INTEGER NOT NULL,
            description TEXT,
            image TEXT,
            instagram INTEGER DEFAULT 0,
            tiktok INTEGER DEFAULT 0,
            youtube INTEGER DEFAULT 0
        )
    )");
}

void addUser(const Request &req, Response &res, sqlite::Connection &db) {
    setCORS(res);
    if (req.method == "OPTIONS") {
        res.status = 200;
        return;
    }
    
    try {
        cerr << "[addUser] Request body: " << req.body << endl;
        auto body = json::parse(req.body);
        string name = body.value("name", "");
        string email = body.value("email", "");
        string password = body.value("password", "");

        cerr << "[addUser] Parsed name: " << name << ", email: " << email << endl;

        if (name.empty() || email.empty() || password.empty()) {
            cerr << "[addUser] Missing required fields" << endl;
            res.status = 400;
            res.set_content(R"({"success":false,"message":"Missing required fields"})", "application/json");
            return;
        }

        auto existing = db.Query("SELECT id FROM users WHERE email = ?", email);
        if (existing.Next()) {
            cerr << "[addUser] Email already in use: " << email << endl;
            res.status = 409;
            res.set_content(R"({"success":false,"message":"Email already in use"})", "application/json");
            return;
        }

        string salted = "MASBdjbcaksjbhfBHsjkabfkjdsíbk" + password;
        vector<unsigned char> hash(picosha2::k_digest_size);
        picosha2::hash256(salted.begin(), salted.end(), hash.begin(), hash.end());
        string hashed_password = picosha2::bytes_to_hex_string(hash.begin(), hash.end());

        cerr << "[addUser] Hashed password: " << hashed_password << endl;

        db.Statement("INSERT INTO users (name, email, password) VALUES (?,?,?)",
                     name, email, hashed_password);

        cerr << "[addUser] User inserted successfully" << endl;
        res.set_content(R"({"success":true,"message":"User created successfully"})", "application/json");
    } catch (const exception &e) {
        cerr << "[addUser] error: " << e.what() << endl;
        res.status = 500;
        res.set_content(R"({"success":false,"message":"Internal Server Error"})", "application/json");
    }
}

void getUsers(const Request &req, Response &res, sqlite::Connection &db) {
    setCORS(res);
    if (req.method == "OPTIONS") {
        res.status = 200;
        return;
    }

    try {
        auto result = db.Query("SELECT id, name, email, admin FROM users");
        json users = json::array();

        while (result.Next()) {
            users.push_back({
                {"id", result.Get<int>(0)},
                {"name", result.Get<string>(1)},
                {"email", result.Get<string>(2)},
                {"admin", result.Get<int>(3) != 0}
            });
        }

        res.set_content(users.dump(), "application/json");
    } catch (const exception &e) {
        cerr << "getUsers error: " << e.what() << endl;
        res.status = 500;
        res.set_content(R"({"success":false,"message":"Internal Server Error"})", "application/json");
    }
}

void addProduct(const Request &req, Response &res, sqlite::Connection &db) {
    setCORS(res);
    if (req.method == "OPTIONS") {
        res.status = 200;
        return;
    }

    auto file = req.get_file_value("file");
    if (file.filename.empty()) {
        res.status = 400;
        res.set_content(R"({"success":false,"message":"No file uploaded"})", "application/json");
        return;
    }

    std::string savedFilename;
    if (!app::saveUploadedFile(file, "../silly-little-cats/public/images", savedFilename)) {
        res.status = 500;
        res.set_content(R"({"success":false,"message":"Failed to save file"})", "application/json");
        return;
    }

    try {
        auto metadataPart = req.get_file_value("metadata");
        auto metadataStr = metadataPart.content;

        if (metadataStr.empty()) {
            res.status = 400;
            res.set_content(R"({"success":false,"message":"Missing metadata"})", "application/json");
            return;
        }

        auto body = json::parse(metadataStr);

        std::string name = body.value("name", "");
        std::string description = body.value("description", "");
        int price = body.value("price", 0);
        int instagram = body.value("instagram", 0);
        int tiktok = body.value("tiktok", 0);
        int youtube = body.value("youtube", 0);

        if (name.empty() || price <= 0) {
            res.status = 400;
            res.set_content(R"({"success":false,"message":"Missing required product fields"})", "application/json");
            return;
        }

        db.Statement("INSERT INTO celebrity_cats (name, price, description, image, instagram, tiktok, youtube) VALUES (?,?,?,?,?,?,?)",
                     name, price, description, savedFilename, instagram, tiktok, youtube);

        res.set_content(R"({"success":true,"message":"Product added"})", "application/json");
    } catch (const std::exception &e) {
        cerr << "upload handler error: " << e.what() << endl;
        res.status = 500;
        res.set_content(R"({"success":false,"message":"Internal Server Error"})", "application/json");
    }
}


void getProducts(const Request &req, Response &res, sqlite::Connection &db) {
    setCORS(res);
    if (req.method == "OPTIONS") {
        res.status = 200;
        return;
    }

    try {
        auto result = db.Query("SELECT id, name, price, description, image, instagram, tiktok, youtube FROM celebrity_cats");
        json products = json::array();

        while (result.Next()) {
            products.push_back({
                {"id", result.Get<int>(0)},
                {"name", result.Get<string>(1)},
                {"price", result.Get<int>(2)},
                {"description", result.Get<string>(3)},
                {"image", result.Get<string>(4)},
                {"instagram", result.Get<int>(5)},
                {"tiktok", result.Get<int>(6)},
                {"youtube", result.Get<int>(7)}
            });
        }

        res.set_content(products.dump(), "application/json");
    } catch (const exception &e) {
        cerr << "getProducts error: " << e.what() << endl;
        res.status = 500;
        res.set_content(R"({"success":false,"message":"Internal Server Error"})", "application/json");
    }
}

void deleteProduct(const Request &req, Response &res, sqlite::Connection &db) {
    setCORS(res);
    if (req.method == "OPTIONS") {
        res.status = 200;
        return;
    }

    try {
        if (!req.has_param("id")) {
            res.status = 400;
            res.set_content(R"({"success":false,"message":"Missing product ID"})", "application/json");
            return;
        }

        int productId = stoi(req.get_param_value("id"));
        auto result = db.Query("SELECT id FROM celebrity_cats WHERE id = ?", productId);
        if (!result.Next()) {
            res.status = 404;
            res.set_content(R"({"success":false,"message":"Product not found"})", "application/json");
            return;
        }

        db.Statement("DELETE FROM celebrity_cats WHERE id = ?", productId);
        res.set_content(R"({"success":true,"message":"Product deleted"})", "application/json");
    } catch (const exception &e) {
        cerr << "deleteProduct error: " << e.what() << endl;
        res.status = 500;
        res.set_content(R"({"success":false,"message":"Internal Server Error"})", "application/json");
    }
}

void loginUser(const Request &req, Response &res, sqlite::Connection &db, const std::string& secret) {
    setCORS(res);
    if (req.method == "OPTIONS") {
        res.status = 200;
        return;
    }

    try {
        auto body = json::parse(req.body);
        string email = body.value("email", "");
        string password = body.value("password", "");

        if (email.empty() || password.empty()) {
            res.status = 400;
            res.set_content(R"({"success":false,"message":"Missing email or password"})", "application/json");
            return;
        }

        auto query = db.Query("SELECT id, password FROM users WHERE email = ?", email);
        if (!query.Next()) {
            res.status = 401;
            res.set_content(R"({"success":false,"message":"Invalid credentials"})", "application/json");
            return;
        }

        int userId = query.Get<int>(0);
        string storedHash = query.Get<string>(1);

        string salted = "MASBdjbcaksjbhfBHsjkabfkjdsíbk" + password;
        vector<unsigned char> hash(picosha2::k_digest_size);
        picosha2::hash256(salted.begin(), salted.end(), hash.begin(), hash.end());
        string hashed_password = picosha2::bytes_to_hex_string(hash.begin(), hash.end());

        if (hashed_password != storedHash) {
            res.status = 401;
            res.set_content(R"({"success":false,"message":"Invalid credentials"})", "application/json");
            return;
        }

        string token = createToken(userId, 3600 * 24, secret);

        json response = {
            {"success", true},
            {"token", token}
        };
        res.set_content(response.dump(), "application/json");
    } catch (const exception &e) {
        cerr << "loginUser error: " << e.what() << endl;
        res.status = 500;
        res.set_content(R"({"success":false,"message":"Internal Server Error"})", "application/json");
    }
}
}

int main() {
    try {
        sqlite::Connection db(app::DB_FILE);
        app::initializeDatabase(db);
        const std::string SECRET_KEY = "your-very-secret-key";

        Server svr;

        svr.Options(".*", [](const Request& req, Response& res) {
            app::setCORS(res);
            res.status = 200;
        });

        svr.Post("/api/add-user", [&](const Request& req, Response& res) {
            app::addUser(req, res, db);
        });

        svr.Get("/api/get-users", [&](const Request& req, Response& res) {
            app::getUsers(req, res, db);
        });

        svr.Post("/api/upload", [&](const Request& req, Response& res) {
            int userId;
            if (!app::requireAuth(req, res, SECRET_KEY, userId)) return;
            app::addProduct(req, res, db);
        });        

        svr.Get("/api/get-products", [&](const Request& req, Response& res) {
            app::getProducts(req, res, db);
        });

        svr.Delete("/api/delete-product", [&](const Request& req, Response& res) {
            app::deleteProduct(req, res, db);
        });

        svr.Post("/api/login", [&](const Request& req, Response& res) {
            app::loginUser(req, res, db, SECRET_KEY);
        });

        svr.Get("/api/validate-token", [&](const Request& req, Response& res) {
            app::validateTokenEndpoint(req, res, SECRET_KEY);
        });

        cout << "Server running on port 3000\n";
        svr.listen("0.0.0.0", 3000);

    } catch (const exception &e) {
        cerr << "Fatal: " << e.what() << endl;
        return 1;
    }
    return 0;
}