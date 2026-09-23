import cv2
import requests

ESP32_URL = "http://192.168.4.1"
STREAM_URL = ESP32_URL + "/mjpeg/1"
QR_URL = ESP32_URL + "/qr"

cap = cv2.VideoCapture(STREAM_URL)

if not cap.isOpened():
    print("无法连接 ESP32-CAM")
    exit()

detector = cv2.QRCodeDetector()

last_data = ""

def is_product_code(data):
    groups = data.split("+")
    return len(groups) == 4 and all(len(group) == 3 and group.isdigit() for group in groups)

while True:
    ret, frame = cap.read()

    if not ret:
        print("无法读取摄像头")
        break

    data, points, _ = detector.detectAndDecode(frame)

    if data and is_product_code(data):
        print("识别到二维码:", data)

        # 防止同一个二维码每一帧都发送
        if data != last_data:
            try:
                response = requests.post(
                    QR_URL,
                    data=data.encode("utf-8"),
                    timeout=2
                )

                if response.ok:
                    print("已发送到 ESP32-CAM")
                    last_data = data
                else:
                    print("ESP32-CAM 返回:", response.status_code)

            except requests.RequestException as e:
                print("发送失败:", e)

        # 画二维码框
        if points is not None:
            points = points.astype(int)

            for i in range(4):
                p1 = tuple(points[0][i])
                p2 = tuple(points[0][(i + 1) % 4])
                cv2.line(
                    frame,
                    p1,
                    p2,
                    (0, 255, 0),
                    2
                )

        cv2.putText(
            frame,
            data,
            (30, 50),
            cv2.FONT_HERSHEY_SIMPLEX,
            1,
            (0, 255, 0),
            2
        )
    elif data:
        print("二维码格式错误，应为 DDD+DDD+DDD+DDD:", data)

    cv2.imshow("QR Code Scanner", frame)

    if cv2.waitKey(1) & 0xFF == ord("q"):
        break

cap.release()
cv2.destroyAllWindows()