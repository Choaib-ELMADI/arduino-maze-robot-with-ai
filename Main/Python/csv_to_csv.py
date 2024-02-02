import pandas as pd


def translate_csv(input_csv, output_csv):
    df = pd.read_csv(input_csv, header=0)
    df.columns = ["Obstacles", "Control"]

    df[["Front", "Right", "Left"]] = pd.DataFrame(
        df["Obstacles"].apply(eval).tolist(), index=df.index
    )

    df[["Steering", "Driving"]] = pd.DataFrame(
        df["Control"].apply(eval).tolist(), index=df.index
    )

    df = df.drop(["Obstacles", "Control"], axis=1)

    df.to_csv(output_csv, index=False)


input_csv_name = "drive__logs-2.csv"
output_csv_name = "driving__logs__2.csv"
csv_path = "C:\\Users\\Choaib ELMADI\\Downloads\\D.I.F.Y\\Electronics\\Robotics\\Labyrinth Resolver\\Programs\\Model\\data\\"

translate_csv(csv_path + input_csv_name, csv_path + output_csv_name)
