import collect_data as cd
import serial

print("Loading...")
bluetooth = serial.Serial(port="COM8", baudrate=9600, timeout=1)
print("BT Connected")

obstacle_distances = []
control_values = []
data_counter = 0
set_size = 50

while True:
    if bluetooth.in_waiting > 0:
        ####! Reading...... !###
        data = bluetooth.readline().decode("utf-8").strip()
        ####! Done Reading. !###

        if data != "WAITING":
            values = data.split(",")

            if len(values) == 6:
                ####! Great Values !###
                isRecording = int(values[0])
                front = int(values[1])
                right = int(values[2])
                left = int(values[3])
                steeringValue = int(values[4])
                drivingValue = int(values[5])

                if isRecording >= 500:
                    if data_counter >= set_size:
                        ####! Save Logs. !###
                        cd.save_logs()
                        data_counter = 0
                    else:
                        ####! Recording..... !###
                        obstacle_distances = [front, right, left]
                        control_values = [steeringValue, drivingValue]
                        cd.save_data(obstacle_distances, control_values)
                        data_counter += 1
                """ else:
                    ####! Not Recording. !###
                    print("Not Recording.", end=" ") """

        """ else:
            ####! Waiting.... !###
            print(data) """
