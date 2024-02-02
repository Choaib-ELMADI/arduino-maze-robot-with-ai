from datetime import datetime
import pandas as pd
import time
import os

global obstacles_list, control_list, folder_counter
obstacles_list = []
control_list = []

folder_counter = 0
counter = 0

my_dir = os.path.join(os.getcwd(), "Collected Data")
while os.path.exists(os.path.join(my_dir, f"obstacles-set-{ str(folder_counter) }")):
    folder_counter += 1

new_path = my_dir + f"/obstacles-set-{ str(folder_counter) }"
os.makedirs(new_path)


def save_data(obstacle_distances, control_values):
    global obstacles_list, control_list

    now = datetime.now()
    timestamp = str(datetime.timestamp(now)).replace(".", "")
    file_name = os.path.join(new_path, f"obstacles__{timestamp}.txt")

    with open(file_name, "w") as file:
        file.write(",".join(str(distance) for distance in obstacle_distances))

    obstacles_list.append(obstacle_distances)
    control_list.append(control_values)


def save_logs():
    global obstacles_list, control_list, folder_counter
    raw_data = {"Obstacles": obstacles_list, "Control": control_list}

    df = pd.DataFrame(raw_data)
    df.to_csv(
        os.path.join(my_dir, f"logs-{str(folder_counter)}.csv"),
        index=False,
        header=False,
    )

    print(f"Logs saved, { len(obstacles_list) } txt sets")
    obstacles_list.clear()
    control_list.clear()
    folder_counter += 1


if __name__ == "__main__":
    for x in range(4):
        save_data([35, 8, 8], [522, 630])
        time.sleep(0.5)

    save_logs()
