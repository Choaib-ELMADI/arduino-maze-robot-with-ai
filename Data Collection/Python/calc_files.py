import os


def count_files_in_folder(folder_path):
    try:
        files = os.listdir(folder_path)
        files = [
            file for file in files if os.path.isfile(os.path.join(folder_path, file))
        ]
        return len(files)

    except FileNotFoundError:
        print(f"Error: Folder not found - {folder_path}")
        return None


folder_number = 6
set_number = 7
folder_path = f"C:\\Users\\Choaib ELMADI\\Downloads\\D.I.F.Y\\Electronics\\Robotics\\Labyrinth Resolver\\Programs\\1- Data Collection\\Collected Data\\set-{set_number}\\obstacles-set-{folder_number}"
file_count = count_files_in_folder(folder_path)

if file_count is not None:
    print(f"Number of files in 'obstacles-set-{folder_number}': {file_count}")
