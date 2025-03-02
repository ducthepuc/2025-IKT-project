require "kemal"
require "file_utils"
require "json"

directory = "uploads"
FileUtils.mkdir_p(directory) unless Dir.exists?(directory)

Kemal.config.public_folder = "uploads" # Allow image serving

post "/api/upload" do |env|
    name = env.params.body["name"]?.to_s
    price = env.params.body["price"]?.to_s
    description = env.params.body["description"]?.to_s
    image = env.params.files["image"]?

    if name.empty? || price.empty? || description.empty?
        env.response.status_code = 400
        { error: "Name, price, and description are required." }.to_json
    else
        file_path = "data/data.txt"

        image_filename = ""
        if image
            image_filename = "#{directory}/#{image.filename}"
            
            File.open(image_filename, "wb") do |file|
                IO.copy(image.tempfile, file)
            end
        end

        File.open(file_path, "a") do |file|
            file.puts("Name: #{name}, Price: #{price}, Description: #{description}, Image: #{image_filename}")
        end

        env.response.content_type = "application/json"
        { success: true, message: "Data saved!", image: image_filename }.to_json
    end
end

get "/api/get-products" do |env|
    products = [] of Hash(String, JSON::Any)

    File.each_line("data/data.txt") do |line|
        if match = line.match(/Name:\s*(.+), Price:\s*(\d+), Description:\s*(.+), Image:\s*(.+)/)
            products << {
                "name"        => JSON::Any.new(match[1].strip),
                "price"       => JSON::Any.new(match[2].to_i),
                "description" => JSON::Any.new(match[3].strip),
                "image"       => JSON::Any.new("/#{match[4].strip}")
            }
        end
    end

    env.response.content_type = "application/json"
    products.to_json
end

before_all do |env|
    env.response.headers["Access-Control-Allow-Origin"] = "*"
    env.response.headers["Access-Control-Allow-Methods"] = "POST, GET, OPTIONS"
    env.response.headers["Access-Control-Allow-Headers"] = "Content-Type"
end

options "/*" do |env|
    env.response.headers["Access-Control-Allow-Origin"] = "*"
    env.response.headers["Access-Control-Allow-Methods"] = "POST, GET, OPTIONS"
    env.response.headers["Access-Control-Allow-Headers"] = "Content-Type"
    env.response.status_code = 204
end

Kemal.run
