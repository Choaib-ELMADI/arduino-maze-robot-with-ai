import os
import shutil


def organize_files(source_folder, target_folder_prefix, files_per_set):
    file_list = sorted(os.listdir(source_folder))

    for i in range(0, len(file_list), files_per_set):
        set_folder = f"{target_folder_prefix}-{i//files_per_set}"
        os.makedirs(set_folder, exist_ok=True)

        for file_name in file_list[i : i + files_per_set]:
            source_path = os.path.join(source_folder, file_name)
            target_path = os.path.join(set_folder, file_name)
            shutil.move(source_path, target_path)


set_number = 7
source_folder = f"C:\\Users\\Choaib ELMADI\\Downloads\\D.I.F.Y\\Electronics\\Robotics\\Labyrinth Resolver\\Programs\\1- Data Collection\\Collected Data\\set-{set_number}\\obstacles-set-{set_number}"
target_folder_prefix = "obstacles-set"
files_per_set = 50

organize_files(source_folder, target_folder_prefix, files_per_set)
