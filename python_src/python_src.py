# -*- coding: utf-8 -*-
from flask import Flask 
app = Flask(__name__) 
@app.route('/') 
def hello_world(): 
 return “Hello World!” #Return this to the web browser 
if __name__ == '__main__': 
 app.run(host='0.0.0.0') #everyone is allowed to access my Server
