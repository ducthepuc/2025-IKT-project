import json
import os
import random
import string
import cgi
from http.server import BaseHTTPRequestHandler, HTTPServer
from urllib.parse import urlparse, parse_qs

class RequestHandler(BaseHTTPRequestHandler):
    def do_GET(self):
        if self.path == '/api/get-products':
            self.send_response(200)
            self.send_header('Content-type', 'application/json')
            self.send_header('Access-Control-Allow-Origin', '*')
            self.send_header('Access-Control-Allow-Methods', 'GET, POST, OPTIONS')
            self.send_header('Access-Control-Allow-Headers', 'Content-Type')
            self.end_headers()
            
            products = []
            try:
                if os.path.exists('data/data.txt') and os.path.getsize('data/data.txt') > 0:
                    with open('data/data.txt', 'r') as file:
                        for line in file:
                            try:
                                match = line.strip().split(', ')
                                product = {
                                    'id': match[0].split(': ')[1],
                                    'name': match[1].split(': ')[1],
                                    'price': int(match[2].split(': ')[1]),
                                    'description': match[3].split(': ')[1],
                                    'image': "/images/" + match[4].split(': ')[1] 
                                }

                                if len(match) > 5:
                                    product['instagram'] = int(match[5].split(': ')[1])
                                if len(match) > 6:
                                    product['tiktok'] = int(match[6].split(': ')[1])
                                if len(match) > 7:
                                    product['youtube'] = int(match[7].split(': ')[1])
                                    
                                products.append(product)
                            except (IndexError, ValueError) as e:
                                print(f"Error parsing line: {line.strip()}")
                                continue
                                
                self.wfile.write(json.dumps(products).encode())
            except Exception as e:
                print(f"Error reading products: {str(e)}")
                self.wfile.write(json.dumps([]).encode())
        else:
            self.send_response(404)
            self.send_header('Content-type', 'text/plain')
            self.send_header('Access-Control-Allow-Origin', '*')
            self.send_header('Access-Control-Allow-Methods', 'GET, POST, OPTIONS')
            self.send_header('Access-Control-Allow-Headers', 'Content-Type')
            self.end_headers()
            self.wfile.write(b'Not Found')

    def do_POST(self):
        if self.path == '/api/upload':
            try:
                env = {'REQUEST_METHOD': 'POST',
                      'CONTENT_TYPE': self.headers['Content-Type']}
                form = cgi.FieldStorage(
                    fp=self.rfile,
                    headers=self.headers,
                    environ=env
                )

                name = form.getvalue('name')
                price = form.getvalue('price')
                description = form.getvalue('description')
                instagram = form.getvalue('instagram', '0')
                tiktok = form.getvalue('tiktok', '0')
                youtube = form.getvalue('youtube', '0')

                frontend_images_path = '../silly-little-cats/public/images'
                if not os.path.exists(frontend_images_path):
                    os.makedirs(frontend_images_path)

                fileitem = None
                if 'image' in form:
                    fileitem = form['image']
                    if hasattr(fileitem, 'filename') and fileitem.filename:
                        filename = os.path.basename(fileitem.filename)
                        filepath = os.path.join(frontend_images_path, filename)
                        
                        file_content = fileitem.file.read()
                        with open(filepath, 'wb') as f:
                            f.write(file_content)
                    else:
                        filename = 'default.jpg'
                else:
                    filename = 'default.jpg'
                    
                if not all([name, price, description]):
                    raise ValueError('Name, price, and description are required.')

                new_id = ''.join(random.choices(string.ascii_letters + string.digits, k=7))

                if not os.path.exists('data'):
                    os.makedirs('data')

                with open('data/data.txt', 'a') as file:
                    file.write(f'ID: {new_id}, Name: {name}, Price: {price}, '
                             f'Description: {description}, Image: {filename}, '
                             f'Instagram: {instagram}, TikTok: {tiktok}, '
                             f'YouTube: {youtube}\n')

                self.send_response(201)
                self.send_header('Content-type', 'application/json')
                self.send_header('Access-Control-Allow-Origin', '*')
                self.send_header('Access-Control-Allow-Methods', 'GET, POST, OPTIONS')
                self.send_header('Access-Control-Allow-Headers', 'Content-Type')
                self.end_headers()
                self.wfile.write(json.dumps({
                    'success': True,
                    'message': 'Upload successful!',
                    'id': new_id,
                    'image': filename
                }).encode())

            except Exception as e:
                self.send_response(400)
                self.send_header('Content-type', 'application/json')
                self.send_header('Access-Control-Allow-Origin', '*')
                self.send_header('Access-Control-Allow-Methods', 'GET, POST, OPTIONS')
                self.send_header('Access-Control-Allow-Headers', 'Content-Type')
                self.end_headers()
                self.wfile.write(json.dumps({
                    'success': False,
                    'message': str(e)
                }).encode())
        else:
            self.send_response(404)
            self.send_header('Content-type', 'text/plain')
            self.send_header('Access-Control-Allow-Origin', '*')
            self.send_header('Access-Control-Allow-Methods', 'GET, POST, OPTIONS')
            self.send_header('Access-Control-Allow-Headers', 'Content-Type')
            self.end_headers()
            self.wfile.write(b'Not Found')

    def do_OPTIONS(self):
        self.send_response(200)
        self.send_header('Access-Control-Allow-Origin', '*')
        self.send_header('Access-Control-Allow-Methods', 'GET, POST, OPTIONS')
        self.send_header('Access-Control-Allow-Headers', 'Content-Type')
        self.end_headers()

def run_server():
    server_address = ('', 3000)
    httpd = HTTPServer(server_address, RequestHandler)
    print('Server running on port 3000...')
    httpd.serve_forever()

if __name__ == '__main__':
    run_server()