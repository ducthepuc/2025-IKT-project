#include "sqlite.hpp"
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include "json.hpp"
#include "httplib.h"

using namespace std;
using json = nlohmann::json;
using namespace httplib;

void initializeDatabase(sqlite::Connection &db) {
    db.Statement("CREATE TABLE IF NOT EXISTS celebrity_cats ("
                 "id INTEGER PRIMARY KEY AUTOINCREMENT, "
                 "name TEXT, "
                 "price INTEGER, "
                 "description TEXT, "
                 "image TEXT, "
                 "instagram INTEGER, "
                 "tiktok INTEGER, "
                 "youtube INTEGER)");
}

void handleUpload(const Request& req, Response& res, sqlite::Connection &db) {
    try {
        res.set_header("Access-Control-Allow-Origin", "*");
        res.set_header("Access-Control-Allow-Methods", "POST, OPTIONS");
        res.set_header("Access-Control-Allow-Headers", "Content-Type");

        if (req.method == "OPTIONS") {
            res.status = 200;
            return;
        }

        string name, description;
        int price = 0, instagram = 0, tiktok = 0, youtube = 0;

        for (const auto &field : req.files) {
            const auto &key = field.first;
            const auto &value = field.second;
            
            if (key == "name") name = value.content;
            else if (key == "description") description = value.content;
            else if (key == "price") price = stoi(value.content);
            else if (key == "instagram") instagram = stoi(value.content);
            else if (key == "tiktok") tiktok = stoi(value.content);
            else if (key == "youtube") youtube = stoi(value.content);
        }

        if (!req.has_file("image")) {
            cerr << "Error: Image file missing!" << endl;
            res.status = 400;
            res.set_content("{\"success\": false, \"message\": \"Image file missing\"}", "application/json");
            return;
        }

        auto image_file = req.get_file_value("image");
        string image_path = "../silly-little-cats/public/images/" + image_file.filename;

        ofstream out(image_path, ios::binary);
        if (!out) {
            throw runtime_error("Failed to open image file for writing.");
        }
        out.write(image_file.content.data(), image_file.content.size());
        out.close();
        string cat_filename= image_file.filename;

        cout << "Extracted Fields:" << endl;
        cout << "Name: " << name << endl;
        cout << "Description: " << description << endl;
        cout << "Price: " << price << endl;
        cout << "Instagram: " << instagram << endl;
        cout << "TikTok: " << tiktok << endl;
        cout << "YouTube: " << youtube << endl;
        cout << "Image Path: " << cat_filename << endl;

        db.Statement("INSERT INTO celebrity_cats (name, price, description, image, instagram, tiktok, youtube) "
                     "VALUES (?,?,?,?,?,?,?)",
                     name, price, description, cat_filename, instagram, tiktok, youtube);

        res.set_content("{\"success\": true, \"message\": \"Upload successful\"}", "application/json");
    } catch (const exception &e) {
        cerr << "Error in handleUpload: " << e.what() << endl;
        res.status = 500;
        res.set_content("{\"success\": false, \"message\": \"Internal Server Error: " + string(e.what()) + "\"}", "application/json");
    }
}

void getProducts(const Request& req, Response& res, sqlite::Connection &db) {
    try {
        res.set_header("Access-Control-Allow-Origin", "*");
        res.set_header("Access-Control-Allow-Methods", "GET, OPTIONS");
        res.set_header("Access-Control-Allow-Headers", "Content-Type");

        if (req.method == "OPTIONS") {
            res.status = 200;
            return;
        }

        sqlite::Result result = db.Query("SELECT * FROM celebrity_cats");
        json products = json::array();

        while (result.Next()) {
            json cat = {
                {"id", result.Get<int>(0)},
                {"name", result.Get<std::string>(1)},
                {"price", result.Get<int>(2)},
                {"description", result.Get<std::string>(3)},
                {"image", result.Get<std::string>(4)},
                {"instagram", result.Get<int>(5)},
                {"tiktok", result.Get<int>(6)},
                {"youtube", result.Get<int>(7)}
            };
            products.push_back(cat);
        }

        res.set_content(products.dump(), "application/json");
    } catch (const std::exception &e) {
        std::cerr << "Error in getProducts: " << e.what() << std::endl;
        res.status = 500;
        res.set_content("{\"success\": false, \"message\": \"Internal Server Error: " + std::string(e.what()) + "\"}", "application/json");
    }
}

void deleteProduct(const Request& req, Response& res, sqlite::Connection &db) {
    try {
        cout << "Trying to delete product..." << endl;
        res.set_header("Access-Control-Allow-Origin", "*");
        res.set_header("Access-Control-Allow-Methods", "DELETE, OPTIONS");
        res.set_header("Access-Control-Allow-Headers", "Content-Type");

        if (req.method == "OPTIONS") {
            res.status = 200;
            return;
        }

        if (!req.has_param("id")) {
            res.status = 400;
            res.set_content("{\"success\": false, \"message\": \"Missing product ID\"}", "application/json");
            return;
        }

        int productId = stoi(req.get_param_value("id"));

        sqlite::Result result = db.Query("SELECT id FROM celebrity_cats WHERE id = ?", productId);

        if (!result.Next()) {
            res.status = 404;
            res.set_content("{\"success\": false, \"message\": \"Product not found\"}", "application/json");
            return;
        }

        db.Statement("DELETE FROM celebrity_cats WHERE id = ?", productId);

        res.set_content("{\"success\": true, \"message\": \"Product deleted successfully\"}", "application/json");

    } catch (const exception &e) {
        cerr << "Error in deleteProduct: " << e.what() << endl;
        res.status = 500;
        res.set_content("{\"success\": false, \"message\": \"Internal Server Error\"}", "application/json");
    }
}



void deleteAllProducts(const Request& req, Response& res, sqlite::Connection &db) {
    try {
        res.set_header("Access-Control-Allow-Origin", "*");
        res.set_header("Access-Control-Allow-Methods", "DELETE, OPTIONS");
        res.set_header("Access-Control-Allow-Headers", "Content-Type");

        if (req.method == "OPTIONS") {
            res.status = 200;
            return;
        }

        db.Statement("DELETE FROM celebrity_cats");

        res.set_content("{\"success\": true, \"message\": \"All products deleted successfully\"}", "application/json");
    } catch (const exception &e) {
        cerr << "Error in deleteAllProducts: " << e.what() << endl;
        res.status = 500;
        res.set_content("{\"success\": false, \"message\": \"Internal Server Error\"}", "application/json");
    }
}

int main() {
    try {
        sqlite::Connection db("cats.db");
        initializeDatabase(db);
        
        Server svr;

        svr.Options(".*", [](const Request&, Response& res) {
            res.set_header("Access-Control-Allow-Origin", "*");
            res.set_header("Access-Control-Allow-Methods", "GET, POST, DELETE, OPTIONS");
            res.set_header("Access-Control-Allow-Headers", "Content-Type");
            res.status = 200;
        });

        svr.Post("/api/upload", [&](const Request& req, Response& res) {
            handleUpload(req, res, db);
        });

        svr.Get("/api/get-products", [&](const Request& req, Response& res) {
            getProducts(req, res, db);
        });

        svr.Delete(R"(/api/delete-product)", [&](const Request& req, Response& res) {
            deleteProduct(req, res, db);
        });

        svr.Delete(R"(/api/delete-all-products)", [&](const Request& req, Response& res) {
            deleteAllProducts(req, res, db);
        });


        std::cout << "Server running on port 3000..." << std::endl;
        svr.listen("0.0.0.0", 3000);
    } catch (const std::exception &e) {
        std::cerr << "Fatal error: " << e.what() << std::endl;
        return 1;
    }

    return 0;
}
