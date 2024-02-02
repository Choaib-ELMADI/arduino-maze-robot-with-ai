from sklearn.tree import DecisionTreeClassifier
from sklearn import tree
import pandas as pd
import warnings

warnings.filterwarnings("ignore", category=UserWarning, module="sklearn")


def visualize_model(data_path, output_file):
    df = pd.read_csv(data_path)
    X = df.drop(["Steering", "Driving"], axis=1)
    y = df[["Steering", "Driving"]]

    model = DecisionTreeClassifier()
    model.fit(X, y)

    tree.export_graphviz(
        model,
        out_file=output_file,
        feature_names=["Front", "Right", "Left"],
        label="all",
        rounded=True,
        filled=True,
    )


if __name__ == "__main__":
    print("Model Visualization")
