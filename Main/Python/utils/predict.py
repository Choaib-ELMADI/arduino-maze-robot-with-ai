def predict(model, obstacle_distances):
    prediction = model.predict([obstacle_distances])
    control_values = prediction[0]
    steering, driving = control_values
    return steering, driving


if __name__ == "__main__":
    print("Making Predictions")
