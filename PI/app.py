from flask import Flask, Response
from camera import generateFrame
from motion import init_motion, cleanup

app = Flask(__name__)


@app.route('/stream')
def stream():
    return Response(
        generateFrame(),
        mimetype='multipart/x-mixed-replace; boundary=frame'
    )


if __name__ == "__main__":
    try:
        init_motion()
        print("Added event")
        app.run(host="0.0.0.0", port=8080, threaded=True)

    except KeyboardInterrupt:
        cleanup()
