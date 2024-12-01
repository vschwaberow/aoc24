def read_lists(filename):
    with open(filename, 'r') as file:
        lines = file.readlines()

    left_list = [int(line.split()[0]) for line in lines]
    right_list = [int(line.split()[1]) for line in lines]

    return left_list, right_list

if __name__ == "__main__":
    left_list, right_list = read_lists('list.txt')

    print(f"left_list = {{{', '.join(map(str, left_list))}}};")
    print(f"right_list = {{{', '.join(map(str, right_list))}}};")
