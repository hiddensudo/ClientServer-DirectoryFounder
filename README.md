# Directory Founder

Directory Founder is a software application for finding the full path to a directory. To use the software application, you need to run 2 terminals. One will be the server, the other will be the client.

## Building the Project

To build the project, follow these steps:

```bash
$ mkdir build
$ cd build
$ cmake ..
$ make
```
If you are using gnome-shell, you can run the project using the `make run` command. This command will execute a script that conveniently opens 2 terminals, one for the server and another for the client. 

If you are not using gnome-shell, then after executing make, you can run `./server` in one terminal and `./client` in another.

## How to use

To find the path to a directory, you need to write the command `/getpath`, after which you specify the starting path and the name of the directory you want to find.

Example:
```
>>> /getpath
Enter PATH: directory
>>> /
Enter WANTED: directory
>>> Downloads    
Client: Full path to the directory -> /home/username/Downloads
```

After some time on the server side, you will be able to see a message from the client with the full path to the file.

## Kill client or server

To kill a client you need to write `#`. After that you will see this message:

```
Connection closed!
Enter y/n to kill server/keep server on
>>> 
```
If you want to continue working with another client, you need to write `n`. After that, the server will wait for another client/
```
Enter y/n to kill server/keep server on
>>> n
SERVER MESSAGE: Connected to the client
```

And if you want to quit and kill the server, you can enter `y` 
```
Enter y/n to kill server/keep server on
>>> y
Server process killed
```
