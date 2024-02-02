from utils.load_model import load_model
from utils.predict import predict
import serial

model_name = "model__2"
model_path = f"C:\\Users\\Choaib ELMADI\\Downloads\\D.I.F.Y\\Electronics\\Robotics\\Labyrinth Resolver\\Programs\\Model\\models\\{model_name}.joblib"
model = load_model(model_path)
print("Model Loaded.")

print("BT Connection...")
bluetooth = serial.Serial(port="COM8", baudrate=9600, timeout=1_000)
print("BT Connected.")

control_data = ""

while True:
    if bluetooth.in_waiting > 0:
        ####! Reading...... !###
        obstacles_data = bluetooth.readline().decode("utf-8").strip()
        ####! Done Reading. !###

        values = obstacles_data.split(",")
        if len(values) == 3:
            front = abs(int(values[0]))
            right = abs(int(values[1]))
            left = abs(int(values[2]))

            steering, driving = predict(model, [front, right, left])
            control_data = f"{steering},{driving}"

    bluetooth.write((control_data + "\n").encode("utf-8"))
