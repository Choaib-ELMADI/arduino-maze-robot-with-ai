import warnings
import joblib

warnings.filterwarnings("ignore", category=UserWarning, module="sklearn")


def load_model(model_path):
    model = joblib.load(model_path)
    return model


if __name__ == "__main__":
    print("Loading Model")
