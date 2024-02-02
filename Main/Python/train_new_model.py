from sklearn.tree import DecisionTreeClassifier
import pandas as pd
import joblib
import warnings

warnings.filterwarnings("ignore", category=UserWarning, module="sklearn")


def train_new_model(data_path, model_output):
    df = pd.read_csv(data_path)
    X = df.drop(["Steering", "Driving"], axis=1)
    y = df[["Steering", "Driving"]]

    model = DecisionTreeClassifier()
    model.fit(X, y)
    joblib.dump(model, model_output)

    print(f"Model Trained: '{model_output}")


new_data_name = "driving__logs__2"  #! Current: 2
new_data_path = f"C:\\Users\\Choaib ELMADI\\Downloads\\D.I.F.Y\\Electronics\\Robotics\\Labyrinth Resolver\\Programs\\Model\\data\\{new_data_name}.csv"

new_model_name = "model__2"  #! Current: 2
model_output = f"C:\\Users\\Choaib ELMADI\\Downloads\\D.I.F.Y\\Electronics\\Robotics\\Labyrinth Resolver\\Programs\\Model\\models\\{new_model_name}.joblib"


train_new_model(new_data_path, model_output)
