//To run copy this: g++ Solution.cpp -ljsoncpp
//to get output: ./a.out

#include<bits/stdc++.h>
#include<jsoncpp/json/value.h>
#include<jsoncpp/json/json.h>
#include<fstream>

#define ll long long
using namespace std;

class Task {

public:
        ll cost;
        ll start;       //start time of task
        ll end;         //end time of task
        ll w;           //name of worker used in the task
        ll left;        //no. of dependencies left
        vector<string> depends;     //dependency array
        vector<string> childs;      //child array

        Task(ll cost, ll start, ll end, ll w, ll left,vector<string> temp)
        {
                this->cost = cost;
                this->start = start;
                this->end = end;
                this->w = w;
                this->left = left;

                this->depends=temp;
                this->childs=temp;
        }
};

struct CompareCost {
        bool operator()(Task* const& t1, Task* const& t2)
        {
                return t1->cost > t2->cost;
        }
};

struct CompareWorker {
        bool operator()(pair<ll, ll> const& w1, pair<ll, ll> const& w2)
        {
                return w1.first < w1.first;
        }
};


ll minStartTime=INT_MAX;

//Main call to execute the workflows
void Process_workflows(vector<vector<Task*>> &Workflows, int workers,unordered_map<string,Task*> NameTotask)
{
    //Max-Heap of Tasks
    priority_queue<Task*, vector<Task*>, CompareCost> pq;

    for(auto &W: Workflows)
    {
        for(auto &T: W)
        {
             Task* temp=T;
        
             if(temp->depends.size()==0)
                 pq.push(temp);
        }
    }

    //Min-Heap of Workers
    priority_queue<pair<ll, ll>, vector<pair<ll, ll>>, CompareWorker> min_worker;

    int i=1;

    //Each member of Min-Heap of Workers is initialized with the minimum scheduled time among the workflows
    while(workers--)
        min_worker.push({minStartTime,i++});

    //Execution of tasks until Max-Heap becomes empty
    while (!pq.empty())
    {
                //Extraction of minimum current time from Min-Heap of Workers
                pair<ll, ll> curr_min_worker = min_worker.top();
                min_worker.pop();

                //This store all maximum elements from the Max-Heap of Tasks which doesn't follow the conditions
                queue<Task*> temp_pq;

                //curr_task is the 'task' which will be executed now
                Task* curr_task = pq.top();
                int f=0;

                //Storing in temp_pq which doesn't satisfy the condition
                while(!pq.empty()&&(curr_task->start)>curr_min_worker.first)
                {
                    temp_pq.push(curr_task);
                    pq.pop();
                    if(!pq.empty())
                    curr_task=pq.top();
                    else
                    {
                        Task* t;
                        ll mini=INT_MAX;

                        while(!temp_pq.empty())
                        {
                            if(temp_pq.front()->start<mini)
                            {
                                mini=temp_pq.front()->start;
                                t=temp_pq.front();
                            }

                            temp_pq.pop();
                        }

                        curr_task=t;
                        curr_min_worker.first=mini;
                        f=1;
                        break;
                    }
                }

                if(f==0)
                pq.pop();
                

                curr_task->start = curr_min_worker.first;  //updating the start time of current task
                curr_task->w = curr_min_worker.second;     //updating the worker number of current task
                curr_task->end = curr_min_worker.first + curr_task->cost;          //updating the end time of current task  
                curr_min_worker.first = curr_task->end;                            //updating the minimum current worker value

                //Updating all its child dependencies and start value
                for (auto &x : curr_task->childs)
                {
                        Task* child=NameTotask[x];
                        child->start=max(child->start,curr_task->end);
                        child->left = child->left - 1;

                        //child stack is pushed into the queue only if left == 0 i.e. dependency array size is 0
                        if (child->left == 0)
                                pq.push(child);
                }

                //current minimum worker is pushed into the Min-Heap of Workers
                min_worker.push(curr_min_worker);

                //Elements in temp_pq is now pushed into Max-Heap of Tasks
                while(!temp_pq.empty())
                {
                    if(temp_pq.front()!=curr_task)
                    pq.push(temp_pq.front());
                    temp_pq.pop();
                }

    }  

}

int main()
{
    //Using fstream to get the file pointer in "file"
    ifstream file("file.json");
    Json::Value actualJson;
    Json::Reader reader;

    freopen("output.json", "w", stdout);

    //Using reader we are parsing the Json
    reader.parse(file, actualJson);

    //2-D vector of tasks where each row indicates a single workflow(1-D vector of tasks)
    vector<vector<Task*>> Workflows;

    ll i=1;

    //map used to convert task name to Task node for ex: "WF1T1"-> node pointing to task-1 of workflow-1
    unordered_map<string,Task*> NameToTask;
    
    //contains scheduled time of each workflow
    vector<ll> scheduledAt;

    //Extracting data from file.json
    for(auto &W: actualJson["workflows"])
    {
        scheduledAt.push_back((ll)W["scheduled_at"].asInt());
        minStartTime=min(minStartTime,scheduledAt.back());

        ll j=1;

        vector<Task*> tasks;
        unordered_map<string,Task*> NameTotask;

        for(auto &T: W["tasks"])
        {
            vector<string> temp;
            Task* NewTask = new Task((ll)T["cost"].asInt(), scheduledAt[scheduledAt.size()-1], INT_MAX, INT_MAX, T["dependencies"].size(),temp);

            for(auto &d: T["dependencies"])
            {
                string str=d.toStyledString();
                str.pop_back();
                str.pop_back();
                str=str.substr(1);
                string parentTask="W"+to_string(i)+str;
                NewTask->depends.push_back(parentTask);

                Task* tempTask=NameTotask[parentTask];
                tempTask->childs.push_back("W"+to_string(i)+"task"+to_string(j));
                    

            }

            NameTotask["W"+to_string(i)+"task"+to_string(j)]=NewTask;
            j++;
            tasks.push_back(NewTask);
            //cout<<tasks.back()->cost;

            for(auto &x: NameTotask)
            {
                NameToTask[x.first]=x.second;
            }
        }

        Workflows.push_back(tasks);


        i++;

    } 

    //Call to main function to execute the tasks
    Process_workflows(Workflows,2,NameToTask);

    //Printing the output in the output.json file
    cout<< "[\n";

    for(i=0;i<Workflows.size();i++)
    {
        cout<<"\t{\n";
        cout<<"\t\t\"name\": \"workflow";
        cout<<(i+1);
        cout<<"\",\n";
        cout<<"\t\t\"scheduled_at\": \"";
        cout<< scheduledAt[i];
        cout<< "\",\n";


        ll completedAt=INT_MIN;

        for(ll j=0;j<Workflows[i].size();j++)
        {
            completedAt=max(Workflows[i][j]->end,completedAt);
        }

        cout<<"\t\t\"completed_at\": \"";
        cout<< completedAt;
        cout<<"\",\n";
        cout<<"\t\t\"tasks\": [\n";

        int m=Workflows[i].size();
        
        for(ll j=0;j<m;j++)
        {
            Task* t=Workflows[i][j];
            cout<<"\t\t{\n";
            cout<<"\t\t\t\"name\": \"task";
            cout<<(j+1);
            cout<<"\",\n";
            cout<<"\t\t\t\"worker\": \"w";
            cout<<t->w;
            cout<<"\",\n";
            cout<<"\t\t\t\"started_at\": ";
            cout<<t->start;
            cout<<",\n";
            cout<<"\t\t\t\"completed_at\": ";
            cout<<t->end;
            cout<<"\n";  
            cout<<"\t\t}";

            if(j<m-1)
                cout<<",";

            cout<<"\n";        
        }

        cout<<"\t\t]\n";
        cout<<"\t}";

         if(i!=(int)Workflows.size()-1)
                cout<<",";

            cout<<"\n";

    }  

    cout<< "]\n";

    return 0;

}