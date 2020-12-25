import sys
import os
from datetime import date


TODOFILE = 'todo.txt'
DONEFILE = 'done.txt'

if TODOFILE not in os.listdir() and DONEFILE not in os.listdir():
    file1 = open('todo.txt', 'w+')
    file2 = open('done.txt', 'w+')

    file1.close()
    file2.close()

argvs = sys.argv

def printlist():
    count = []
    with open(TODOFILE, 'r') as f:
        for task in f:
            if task != '\n':
                count.append(task)
        f.close()
    
    if count == []:
        print("There are no pending todos!")
        exit()

    for i, task in enumerate(count):
        print("[{}] {}".format(len(count) - i, task), end="")

def add_to_list(argvs):
    try:
        task_list = []
        with open(TODOFILE, 'r') as f:
            for line in f:
                task_list.append(line)
            task_list = [argvs + "\n"] + task_list
            print('Added todo: \"{}\"'.format(argvs))
            with open(TODOFILE, 'w') as file:
                for i in task_list:
                    file.write(i)
                file.close()
            f.close()
    except FileNotFoundError:
        print("File not found")

def delete_todo(index, done=False):
    try:
        string = ""
        task_list = []
        with open(TODOFILE, 'r') as f:
            for line in f:
                task_list.append(line)
            if int(index) <= 0 or int(index) > len(task_list):
                if done:
                    print("Error: todo #{} does not exist.".format(index))
                else:
                    print("Error: todo #{} does not exist. Nothing deleted.".format(index))
                exit()
            string += task_list.pop(len(task_list) - int(index))
            with open(TODOFILE, 'w') as file:
                for i in task_list:
                    file.write(i)
                file.close()
            f.close()
        return string
    except FileNotFoundError:
        print("File not found")

def done_todo(index):
    try:
        line = "x " + str(date.today()) + " " + delete_todo(index, True)
        with open(DONEFILE, 'a') as f:
            f.write(line)
            f.close()
    except FileNotFoundError:
        print('Added todo: "{}"'.format(argvs))

    
#**************************************************************
#**************************************************************
if len(argvs) == 1 or argvs[1] == "help":
    print('''Usage :-
$ ./todo add "todo item"  # Add a new todo
$ ./todo ls               # Show remaining todos
$ ./todo del NUMBER       # Delete a todo
$ ./todo done NUMBER      # Complete a todo
$ ./todo help             # Show usage
$ ./todo report           # Statistics''')
else:
    main_arg = argvs[1]
    if main_arg == 'ls':
        printlist()
    elif main_arg == 'add':
        if len(argvs) == 3:
            add_to_list(argvs[-1])
        else:
            print("Error: Missing todo string. Nothing added!")
    elif main_arg == 'del':
        if len(argvs) == 3:
            delete_todo(argvs[-1])
            print("Deleted todo #{}".format(argvs[-1]))
        else:
            print("Error: Missing NUMBER for deleting todo.")
    elif main_arg == 'done':
        if len(argvs) == 3:
            done_todo(argvs[-1])
            print("Marked todo #{} as done.".format(argvs[-1]))
        else:
            print("Error: Missing NUMBER for marking todo as done.")
    elif main_arg == 'report':
        done = open(DONEFILE, 'r')
        todo = open(TODOFILE, 'r')

        done_count = 0
        todo_count = 0
        for i in done:
            if i != "":
                done_count += 1
        for i in todo:
            if i != " ":
                todo_count += 1
        print(str(date.today()), "Pending :", todo_count, "Completed :", done_count)
    else:
        print("No valid command!")