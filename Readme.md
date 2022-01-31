**INSTABASE ENCIPHER PROBLEM STATEMENT-2 (Distributed Workflow Processing)**

**Solution Overview**
The main idea of this solution is to provide approximately same execution time for each workflow so that the median of execution time of all workflows is minimized. ​

To do that, we create "n" number of queues ( n = number of workers ) which will store tasks with their start and end time. Also, we maintain a data structure which will distribute the tasks to worker queues available at current time by extracting that "task" which has maximum cost and by inserting into the current worker queue(which has minimum starting time) so that, tasks which has maximum cost and without any dependencies left are finished first for every workflow. And, the data structure which will store "task" are connected like a acyclic graph such that each parent task executes before its child task.​

**Components:**

 1. Class Task: Each object of this class will store information of one task provided in the input as well as information about its workflow to which it                          belongs to. Also, its pointer will connect to all its child tasks.
 2. Min-Heap of Worker Queue: It will store current time available to start any task and by extracting from it gives a queue which has minimum current time.
 
 3. Max-Heap of Tasks: This will help in extracting the maximum element(having maximum cost) whose scheduled time is less than or equal to minimum current                               time(which we got from Min-Heap of Worker Queue) and all the processing of its dependencies are finished.

**Initial Steps:**

- First we form a acyclic graph of objects(which belongs to class Task) where each parent task is connected to all its child tasks through array pointer  of         Task.
- Then we form a Min-Heap of Worker Queue which store the start time. (Initially all queue will have value equal to minimum scheduled time from the workflows       given).
- After that, we create a Max-Heap of objects of type Task class where top will have the maximum cost. We will push an object into this queue only if the size       of dependencies array is equal to zero. 

Instructions for setup:​

1. To install "jsoncpp" as a dev dependency:  In my Linux OS, I have installed it by running the following command in the terminal:  ​

               sudo apt install libjsoncpp-dev
2. After its installation, we can run our code by going to the directory in which all the files are present and running:​

               g++ Solution.cpp -ljsoncpp
3. And to get the ouput in output.json file run: 
               
               ./a.out
Results will automatically updated in output.json file.​
