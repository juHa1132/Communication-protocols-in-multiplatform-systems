from flask import Flask, request, jsonify
from flask_cors import CORS

app = Flask(__name__)
CORS(app)

distance_data = {"distance": 0}

@app.route('/update_distance', methods=['POST'])
def update_distance():
    global distance_data
    data = request.get_json()
    distance_data['distance'] = data['distance']
    print(distance_data)
    return jsonify(success=True)

@app.route('/get_distance', methods=['GET'])
def get_distance():
    return jsonify(distance_data)

if __name__ == '__main__':
    app.run(host='0.0.0.0', port=5000, debug=True)
