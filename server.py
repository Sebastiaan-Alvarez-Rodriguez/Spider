#!/usr/bin/python3
from http.server import BaseHTTPRequestHandler,HTTPServer
import cgi
import os
import subprocess


PORT_NUMBER = 8080

class MyHandler(BaseHTTPRequestHandler):
    def query_create_result(self, url, title):
        return '<div style="line-height: 1.2; margin-bottom: 20px; color: #222"> \
                    <a href="{0}" style="font-size: normal; line-height: 1.57; color: #222; cursor: pointer;"> \
                        <cite style="font-style: normal; text-decoration: none;">{0}</cite> \
                        <div style="font-size: 20px; line-height: 1.57; color: #1a0dab; cursor: pointer;"> \
                            {1} \
                        </div> \
                    </a> \
                </div>'.format(url, title)

    def query_process(self, query):
        returnstring = '<html><body><center><p><br><p><br><font face=arial size=7 color=darkblue>LIACS Search Results</font></center><div style="margin-left: 180px; font-family: arial,sans-serif;">'
        try:
            output = bytes.decode(subprocess.check_output(['./webquery', query]))
            for url_and_title in output.splitlines():
                url, title = url_and_title.split('\t', 1)
                if len(title) == 0:
                    title = 'No title'
                returnstring += self.query_create_result(url, title)
        except Exception as e: # We get here if we have not crawled yet
            print(str(e))
            returnstring += '<div style="font-size: normal; line-height: 1.57; color: #222">We have not crawled the internet yet. Please try again soon!</div>'

        returnstring += '</div></body></html>'
        return returnstring

    def do_GET(self):
        file_to_fetch = ''
        if self.path == '/':
            file_to_fetch = os.path.join('server_pages', 'search.html')
        elif self.path == '/imagesearch':
            file_to_fetch = os.path.join('server_pages', 'imagesearch.html')
        else:
            self.send_error(404,'File Not Found: {0}'.format(self.path))
            return
        try:
            mimetype='text/html'
            #Open the static file requested and send it
            f = open(file_to_fetch, 'rb')
            self.send_response(200)
            self.send_header('Content-type',mimetype)
            self.end_headers()
            self.wfile.write(f.read())
            f.close()
            return
        except IOError:
            self.send_error(404,'File Not Found: {0}'.format(self.path))

    def do_POST(self):
        if self.path=='/query':
            form = cgi.FieldStorage(fp=self.rfile, headers=self.headers, environ={'REQUEST_METHOD':'POST', 'CONTENT_TYPE':self.headers['Content-Type']})
            print('Your query is: {0}'.format(form['query'].value))
            self.send_response(200)
            self.end_headers()
            self.wfile.write(str.encode(self.query_process(form['query'].value)))
            return          
            
            
try:
    #Create a web server and define the handler to manage the
    #incoming request
    server = HTTPServer(('', PORT_NUMBER), MyHandler)
    print('Started httpserver on port {0}'.format(PORT_NUMBER))
    
    #Wait forever for incoming htto requests
    server.serve_forever()

except KeyboardInterrupt:
    print('^C received, shutting down the web server')
    server.socket.close()