#include<bits/stdc++.h>

using namespace std;

clock_t start;
clock_t ende;

vector<vector<vector<int>>> convert_to_wadj_list(vector<vector<int>> matrix)
{
    vector<vector<vector<int>>> wadj_list(matrix.size()+1);                //this is of size N+1---------->no indexing issues!
    for(int i=0;i<matrix.size();i++)
    {
        for(int j=0;j<matrix.size();j++)
        {
            if(matrix[i][j]!=999999 && i!=j)
            {
                vector<int>tempor;                 //label,distance
                tempor.push_back(j+1);
                tempor.push_back(matrix[i][j]);
                wadj_list[i+1].push_back(tempor);
            }
        }
    }
    return wadj_list;
}

vector<int> modified_bellman(vector<vector<vector<int>>>w_adjlist,int source)
{
    int req=0;
    for(int i=1;i<w_adjlist.size();i++)
    {
        req+=w_adjlist[i].size();
    }
    req=req*w_adjlist.size();
    vector<int> distances(w_adjlist.size(),999999);
    distances[source]=0;
    vector<int> currently_queued(w_adjlist.size(),0);  //if in queue we will say it's 1
    queue<int> to_check;
    to_check.push(source);
    distances[source]=0;
    currently_queued[source]=1;
    int total=1;
    int num_of_relax=0;
    int num_of_relax_that_bought_change=0; 
    while(!to_check.empty() && total!=req)
    {
        total++;
        int x=to_check.front();
        to_check.pop();
        currently_queued[x]=0;
        for(int i=0;i<w_adjlist[x].size();i++)
        {
            num_of_relax+=1;
            if(distances[x]+w_adjlist[x][i][1]<distances[w_adjlist[x][i][0]])
            {
                num_of_relax_that_bought_change+=1;
                distances[w_adjlist[x][i][0]]=distances[x]+w_adjlist[x][i][1];
                if(currently_queued[w_adjlist[x][i][0]]!=1)
                {
                    currently_queued[w_adjlist[x][i][0]]=1;
                    to_check.push(w_adjlist[x][i][0]);
                }
            }

        }
    }
    if(total==req)
    {
        //cout<<-1;
        vector<int>empty_vect(0);
        distances=empty_vect;
    }
    //else
    //{
        //for(int i=1;i<distances.size();i++)
        //{
       //     cout<<distances[i]<<" ";
       // }
       // cout<<num_of_relax<<" "<<num_of_relax_that_bought_change;
    //}
    return distances;
}

vector<int> array_dijkastra(vector<vector<vector<int>>>w_adjlist,int source,vector<int>bell_dist)
{
    vector<int> distances(w_adjlist.size());
    vector<pair<int,int>>checker;
    //priority_queue<pair<int,int>>checker;                               //distance,label

    for(int i=1;i<w_adjlist.size();i++)
    {
        distances[i]=999999;
        //visited_or_not[i]=0;
    }
    int negatives=0;
    distances[source]=0;
    checker.push_back(make_pair(0,source));
    //checker.push(make_pair(0,source));

    while(!checker.empty() && negatives==0)
    {
        //cout<<"hi";
        ///////////
        //int temp_source=(*min_element(checker.begin(),checker.end()))->second;
        //int temp_source=(checker.begin())->second;
        /////////////////////////////////////////////////////
        //checker.erase(checker.begin());
        int mini=checker[0].first;
        int temp_source=checker[0].second;
        for(int k=0;k<checker.size();k++)
        {
            if(checker[k].first<mini)
            {
                mini=checker[k].first;
                temp_source=checker[k].second;
            }
        }
        for(int k=0;k<checker.size();k++)
        {
            if(checker[k].second==temp_source)
            {
                checker.erase(checker.begin()+k);
                break;
            }
        }


        //checker.pop();
        for(int i=0;i<w_adjlist[temp_source].size();i++)
        {
            if(w_adjlist[temp_source][i][1]<0)
                {
                    negatives+=1;
                }
            //cout<<w_adjlist[temp_source][i][0]<<" "<<distances[w_adjlist[temp_source][i][0]]<<"bkjn";
            if(distances[w_adjlist[temp_source][i][0]]!=999999 || distances[temp_source]!=999999)
            {
                if(distances[temp_source]+w_adjlist[temp_source][i][1]<distances[w_adjlist[temp_source][i][0]])
                {
                    //if my destination is already in the set I need to update it and erase and insert again
                    vector<pair<int,int>>::iterator it=find(checker.begin(),checker.end(),make_pair(distances[w_adjlist[temp_source][i][0]],w_adjlist[temp_source][i][1]));
                    //priority_queue<pair<int,int>>::iterator it=checker.find(make_pair(distances[w_adjlist[temp_source][i][0]],w_adjlist[temp_source][i][1]));
                    if(it!=checker.end())
                    {
                        checker.erase(it);
                        //same
                    }
                    distances[w_adjlist[temp_source][i][0]]=distances[temp_source]+w_adjlist[temp_source][i][1];
                    checker.push_back(make_pair(distances[w_adjlist[temp_source][i][0]],w_adjlist[temp_source][i][0]));
                    //checker.push(make_pair(distances[w_adjlist[temp_source][i][0]],w_adjlist[temp_source][i][0]));
                }
            }
        }
    }
    if(negatives!=0)
    {
        cout<<-1<<" ";
    }
    else
    {
        for(int i=1;i<distances.size();i++)
        {
            if(distances[i]!=999999)
            {
                distances[i]+=bell_dist[i]-bell_dist[source];
            }
            cout<<distances[i]<<" ";
        }
    }
    cout<<endl;
    return distances;
}


//binary heap
/*
vector<int> binaryheap_dijkastra(vector<vector<vector<int>>>w_adjlist,int source,vector<int>bell_dist)
{
    vector<int> distances(w_adjlist.size());
    vector<pair<int,int>>checker;                                         //binary heap
    //priority_queue<pair<int,int>>checker;                               //distance,label

    for(int i=1;i<w_adjlist.size();i++)
    {
        distances[i]=999999;
        //visited_or_not[i]=0;
    }
    int negatives=0;
    distances[source]=0;
    checker.push_back(make_pair(0,source));
    //checker.push(make_pair(0,source));

    while(!checker.empty() && negatives==0)
    {
        int temp_source=checker[0].second;      //temp_source is the one with min distance in the heap we have to delete it now i.e perform extract min
        int i=0;
        checker[0].first=999999;
        while((2*i+1)<checker.size()-1)
        {
            if(2*i+1<checker.size()-1)
            {
                if(checker[i].first>checker[2*i+1].first)
                {
                    if(2*i+2<checker.size())
                    {
                        if(checker[2*i+1].first<checker[2*i+2].first)
                        {
                            pair<int,int> temp=checker[2*i+1];
                            checker[2*i+1]=checker[i];
                            checker[i]=temp;
                            i=2*i+1;
                        }
                        else if(checker[2*i+1].first>checker[2*i+2].first)
                        {
                            pair<int,int> temp=checker[2*i+2];
                            checker[2*i+2]=checker[i];
                            checker[i]=temp;
                            i=2*i+2;
                        }
                        else
                        {
                            if(checker[2*i+1].second<checker[2*i+2].second)
                            {
                                pair<int,int> temp=checker[2*i+1];
                                checker[2*i+1]=checker[i];
                                checker[i]=temp;
                                i=2*i+1;
                            }
                            else
                            {
                                pair<int,int> temp=checker[2*i+2];
                                checker[2*i+2]=checker[i];
                                checker[i]=temp;
                                i=2*i+2;
                            }

                        }
                        
                    }
                }
            }
            else
            {
                break;
            }

        }
        checker.erase(checker.begin()+i);

        //checker.pop();
        for(int i=0;i<w_adjlist[temp_source].size();i++)
        {
            if(w_adjlist[temp_source][i][1]<0)
                {
                    negatives+=1;
                }
            //cout<<w_adjlist[temp_source][i][0]<<" "<<distances[w_adjlist[temp_source][i][0]]<<"bkjn";
            if(distances[w_adjlist[temp_source][i][0]]!=999999 || distances[temp_source]!=999999)
            {
                if(distances[temp_source]+w_adjlist[temp_source][i][1]<distances[w_adjlist[temp_source][i][0]])
                {

                    vector<pair<int,int>>::iterator it=find(checker.begin(),checker.end(),make_pair(distances[w_adjlist[temp_source][i][0]],w_adjlist[temp_source][i][1]));
                    //priority_queue<pair<int,int>>::iterator it=checker.find(make_pair(distances[w_adjlist[temp_source][i][0]],w_adjlist[temp_source][i][1]));
                    if(it!=checker.end())
                    {
                        //checker.erase(it);
                        //make it infinity percolate dowm and remove
                        it->first=999999;
                        int hi=it-checker.begin();
                        while((2*hi)+1<checker.size()-1)
                        {

                            if(2*hi+1<checker.size()-1)
                            {
                                if(checker[hi].first>checker[2*hi+1].first)
                                {
                                    if(checker[2*hi+1].first<checker[2*hi+2].first)
                                    {
                                        pair<int,int> temp=checker[2*hi+1];
                                        checker[2*hi+1]=checker[hi];
                                        checker[hi]=temp;
                                        hi=2*hi+1;
                                    }
                                    else if(checker[2*hi+1].first>checker[2*hi+2].first)
                                    {
                                        pair<int,int> temp=checker[2*hi+2];
                                        checker[2*hi+2]=checker[hi];
                                        checker[hi]=temp;
                                        hi=2*hi+2;
                                    }
                                    else
                                    {
                                        if(checker[2*hi+1].second<checker[2*hi+2].second)
                                        {
                                            pair<int,int> temp=checker[2*hi+1];
                                            checker[2*hi+1]=checker[hi];
                                            checker[hi]=temp;
                                            hi=2*hi+1;
                                        }
                                        else
                                        {
                                            pair<int,int> temp=checker[2*hi+2];
                                            checker[2*hi+2]=checker[hi];
                                            checker[hi]=temp;
                                            hi=2*hi+2;
                                        }
    
                                    }
    
                                }
                            }
                        }
                        checker.erase(checker.begin()+hi);
                    }
                    distances[w_adjlist[temp_source][i][0]]=distances[temp_source]+w_adjlist[temp_source][i][1];
                    //push value to be inserted
                    checker.push_back(make_pair(distances[w_adjlist[temp_source][i][0]],w_adjlist[temp_source][i][0]));

                    //percolate up
                    int hi=checker.size()-1;
                    while(hi!=0)
                    {

                        int parent=(hi-1)/2;
                        if(checker[hi].first<checker[parent].first)
                        {
                            pair<int,int> temp=checker[parent];
                            checker[parent]=checker[i];
                            checker[hi]=temp;
                            hi=parent;
                        }
                        else
                        {
                            break;
                        }

                    }

                }
            }
        }
    }
    if(negatives!=0)
    {
        cout<<-1<<" ";
    }
    else
    {
        for(int i=1;i<distances.size();i++)
        {
            if(distances[i]!=999999)
            {
                distances[i]+=bell_dist[i]-bell_dist[source];
            }
            cout<<distances[i]<<" ";
        }
    }
    cout<<endl;
    return distances;
}
*/

vector<int> binaryheap_dijkastra(vector<vector<vector<int>>>w_adjlist,int source,vector<int>bell_dist)
{
    vector<int> distances(w_adjlist.size());
    vector<pair<int,int>>checker;                                         //binary heap
    //priority_queue<pair<int,int>>checker;                               //distance,label

    for(int i=1;i<w_adjlist.size();i++)
    {
        distances[i]=999999;
        //visited_or_not[i]=0;
    }
    int negatives=0;
    distances[source]=0;
    checker.push_back(make_pair(0,source));
    //checker.push(make_pair(0,source));

    while(!checker.empty() && negatives==0)
    {
        int temp_source=checker[0].second;      //temp_source is the one with min distance in the heap we have to delete it now i.e perform extract min
        int i=0;
        checker[0].first=999999;
        while((2*i+1)<=checker.size()-1)
        {  
            if(checker[i].first>checker[2*i+1].first)
            {
                if(2*i+2<=checker.size()-1)
                {    
                    if(checker[2*i+1].first<checker[2*i+2].first)
                    {
                        pair<int,int> temp=checker[2*i+1];
                        checker[2*i+1]=checker[i];
                        checker[i]=temp;
                        i=2*i+1;
                    }
                    else if(checker[2*i+1].first>checker[2*i+2].first)
                    {
                        pair<int,int> temp=checker[2*i+2];
                        checker[2*i+2]=checker[i];
                        checker[i]=temp;
                        i=2*i+2;
                    }
                    else
                    {
                        if(checker[2*i+1].second<checker[2*i+2].second)
                        {
                            pair<int,int> temp=checker[2*i+1];
                            checker[2*i+1]=checker[i];
                            checker[i]=temp;
                            i=2*i+1;
                        }
                        else
                        {
                            pair<int,int> temp=checker[2*i+2];
                            checker[2*i+2]=checker[i];
                            checker[i]=temp;
                            i=2*i+2;
                        }

                    }

                }
                else
                {
                    pair<int,int> temp=checker[2*i+1];
                    checker[2*i+1]=checker[i];
                    checker[i]=temp;
                    i=2*i+1;
                }
            }
            else
            {
                break;
            }
               

        }
        checker.erase(checker.begin()+i);

        //checker.pop();
        for(int i=0;i<w_adjlist[temp_source].size();i++)
        {
            if(w_adjlist[temp_source][i][1]<0)
                {
                    negatives+=1;
                }
            if(distances[w_adjlist[temp_source][i][0]]!=999999 || distances[temp_source]!=999999)
            {
                if(distances[temp_source]+w_adjlist[temp_source][i][1]<distances[w_adjlist[temp_source][i][0]])
                {

                    vector<pair<int,int>>::iterator it=find(checker.begin(),checker.end(),make_pair(distances[w_adjlist[temp_source][i][0]],w_adjlist[temp_source][i][1]));
                    //priority_queue<pair<int,int>>::iterator it=checker.find(make_pair(distances[w_adjlist[temp_source][i][0]],w_adjlist[temp_source][i][1]));
                    if(it!=checker.end())
                    {
                        //checker.erase(it);
                        //make it infinity percolate dowm and remove
                        it->first=999999;
                        int hi=it-checker.begin();
                        while((2*hi)+1<=checker.size()-1)
                        {
                            if((2*hi)+2 <= checker.size()-1)
                            {
                                if(checker[hi].first>checker[2*hi+1].first || checker[hi].first>checker[2*hi+2].first)
                                {
                                    if(checker[2*hi+1].first<checker[2*hi+2].first)
                                    {
                                        pair<int,int> temp=checker[2*hi+1];
                                        checker[2*hi+1]=checker[hi];
                                        checker[hi]=temp;
                                        hi=2*hi+1;
                                    }
                                    else if(checker[2*hi+1].first>checker[2*hi+2].first)
                                    {
                                        pair<int,int> temp=checker[2*hi+2];
                                        checker[2*hi+2]=checker[hi];
                                        checker[hi]=temp;
                                        hi=2*hi+2;
                                    }
                                    else
                                    {
                                        if(checker[2*hi+1].second<checker[2*hi+2].second)
                                        {
                                            pair<int,int> temp=checker[2*hi+1];
                                            checker[2*hi+1]=checker[hi];
                                            checker[hi]=temp;
                                            hi=2*hi+1;
                                        }
                                        else
                                        {
                                            pair<int,int> temp=checker[2*hi+2];
                                            checker[2*hi+2]=checker[hi];
                                            checker[hi]=temp;
                                            hi=2*hi+2;
                                        }
    
                                    }

                                }    
                                else
                                {
                                    break;
                                }
                                
                            }
                            else
                            {
                                if(checker[hi].first>checker[2*hi+1].first)
                                {
                                    pair<int,int> temp=checker[2*hi+1];
                                    checker[2*hi+1]=checker[hi];
                                    checker[hi]=temp;
                                    hi=2*hi+1;
                                }
                                else
                                {
                                    break;
                                }
                                
                            }
                        }
                        checker.erase(checker.begin()+hi);
                    }
                    distances[w_adjlist[temp_source][i][0]]=distances[temp_source]+w_adjlist[temp_source][i][1];
                    //push value to be inserted
                    checker.push_back(make_pair(distances[w_adjlist[temp_source][i][0]],w_adjlist[temp_source][i][0]));

                    //percolate up
                    int hi=checker.size()-1;
                    while(hi!=0)
                    {

                        int parent=(hi-1)/2;
                        if(checker[hi].first<checker[parent].first)
                        {
                           // pair<int,int> temp=checker[parent];
                            int t1= checker[parent].first;
                            int t2= checker[parent].second;
                            checker[parent]=checker[hi];
                           // checker[hi]=temp;
                            checker[hi].first= t1;
                            checker[hi].second= t2;
                            hi=parent;
                        }
                        else
                        {
                            break;
                        }

                    }

                }
            }
        }
    }
    if(negatives!=0)
    {
        cout<<-1<<" ";
    }
    else
    {
        for(int i=1;i<distances.size();i++)
        {
            if(distances[i]!=999999)
            {
                distances[i]+=bell_dist[i]-bell_dist[source];
            }
            cout<<distances[i]<<" ";
        }
    }
    cout<<endl;
    return distances;
}


//binomial heaps
struct bino_node
{
    pair<int,int>noder;       //distance, label
    int rank;
    bino_node *parent;
    bino_node * child;
    bino_node * sib;
};

vector<bino_node*> mergebinoheaps(vector<bino_node*> heap1, vector<bino_node*> heap2)
{
    //we will first take super nodes from each heap and merge them accordingly to make a
    //bigger binary heap
    vector<bino_node*> final;
    vector<bino_node*>::iterator it1 = heap1.begin();
    vector<bino_node*>::iterator it2=heap2.begin();

    while(it1!=heap1.end() && it2!=heap2.end())
    {
        //cout<<"f";
        if((*it1)->rank > (*it2)->rank)
        {
            final.push_back(*it2);
            it2++;
        }
        else
        {
            final.push_back(*it1);
            it1++;
        }
    }
    //cout<<"f4";
    //empty both if anything remains
    while(it1!=heap1.end())
    {
        //cout<<"f1";
        final.push_back(*it1);
        it1++;
    }
    while(it2!=heap2.end())
    {
        //cout<<"f2";
        final.push_back(*it2);
        it2++;
    }

    //now we have a final heap but it has super nodes of same rank we now
    //need to merge these to make unique powers of 2
    //cout<<final.size();

    vector<bino_node*> resultant_heap;
    if(final.size()>1)
    {
        //now we have to merge nodes of same ranks in the heap
        //nodes can have same rank if they come from diff heaps or if the merge operation on two previous nodes made a node
        //of same rank
        //hence we can conclude that atmost 3 nodes have the same rank at a time
        //if two nodes have same rank we merge
        //if 3 nodes have the same rank we have to be careful when merging i.e leave the first one to maintain order
        //we have to keep 3 pointers to check any 3 consecutive nodes
        it1=final.begin();
        it2=final.begin()+1;
        vector<bino_node*>::iterator it3=final.begin()+1;
        //cout<<"jh";
        if(final.size()==2)
        {
            it3=final.end();
        }
        else
        {
            it3++;
        }
        while(it1!=final.end())
        {
            if(it2==final.end())
            {
                if(resultant_heap.size()>0)
                {
                    if(resultant_heap[resultant_heap.size()-1]!=(*it1))
                    {
                        resultant_heap.push_back(*it1);
                    }
                }
                else
                {
                    resultant_heap.push_back(*it1);
                }
                
                break;
            }
            else
            {
                //now there can be several cases
                //case 1 when there is 0 conflict of ranks
                if((*it1)->rank!= (*it2)->rank )
                {
                    //cout<<"b";
                    if(resultant_heap.size()>0)
                    {
                        if(resultant_heap[resultant_heap.size()-1]==(*it1) || resultant_heap[resultant_heap.size()-1]==(*it2))
                        {
                            resultant_heap.pop_back();
                        }
                    }    
                    resultant_heap.push_back(*it1);
                    it1++;
                    it2++;
                    if(it3!=final.end())
                    {
                        it3++;
                    }
                    if(it2==final.end())
                    {
                        resultant_heap.push_back(*it1);
                    }
                    //cout<<"c";
                }

                //case 2 when 3 are case
                else if( it3!=final.end() && ((*it1)->rank)==((*it2)->rank) && ((*it3)->rank)==((*it2)->rank) )
                {
                    if(resultant_heap.size()>0)
                    {
                        if(resultant_heap[resultant_heap.size()-1]==*it1)
                        {
                            resultant_heap.pop_back();
                        }
                    }
                    resultant_heap.push_back(*it1);
                    it1++;
                    it2++;
                    it3++;
                }

                else if((*it1)->rank == (*it2)->rank)
                {
                    if(resultant_heap.size()>0)
                    {
                        if(resultant_heap[resultant_heap.size()-1]==(*it1) || resultant_heap[resultant_heap.size()-1]==(*it2))
                        {
                            resultant_heap.pop_back();
                        }
                    }
                    if(((*it1)->noder).first > ((*it2)->noder).first)
                    {
                        bino_node * dummy=(*it2)->child;
                        (*it1)->parent= (*it2);
                        (*it2)->child= (*it1);
                        (*it1)->sib= dummy;
                        ((*it2)->rank)++;
                        resultant_heap.push_back(*it2);
                        it1+=1;
                        if(it2!=final.end())
                        {
                            it2++;
                        }
                    }
                    else
                    {
                        swap(*it1,*it2);
                        bino_node * dummy=(*it2)->child;
                        (*it1)->parent= (*it2);
                        (*it2)->child= (*it1);
                        (*it1)->sib= dummy;
                        ((*it2)->rank)++;
                        //cout<<((*it1)->noder).second<<" "<<((*it2)->noder).second<<" FFFFFFFFFFF"<<endl;
                        resultant_heap.push_back(*it2);
                        it1+=1;
                        if(it2!=final.end())
                        {
                            it2++;
                        }
                    }
                    if(it3!=final.end())
                    {
                        it3++;
                    }


                }
            }

        }
        return resultant_heap;

    }
    else
    {
        return final;
    }

}

vector<bino_node*> bino_insert(pair<int,int> value,vector<bino_node*>my_binoheap)
{
    //make the required node
    //struct bino_node *node=(struct bino_node *)malloc(sizeof(struct bino_node));
    struct bino_node *node= new bino_node;
    node->noder=value;
    node->rank=0;
    node->sib=NULL;
    node->parent=NULL;
    node->child=NULL;

    //this node is a heap hence just make it a vector so that we can re-use the merge heaps function here
    vector<bino_node*>temp_heap;
    temp_heap.push_back(node);
    //cout<<"hi1";
    
    return (mergebinoheaps(my_binoheap,temp_heap));

}

bino_node* find_min(vector<bino_node*>my_heap)
{
    vector<bino_node*> :: iterator it= my_heap.begin();
    bino_node * miniest=*it;
    while(it!=my_heap.end())
    {
        if(((*it)->noder).first<(miniest->noder).first)
        {
            miniest=*it;
        }
        it++;
    }
    return miniest;
}

vector<bino_node*> bino_deletemin(vector<bino_node*>my_heap)
{
    bino_node* to_del=find_min(my_heap);
    //first make a heap of all elements except the one from which we have to delete
    vector<bino_node *> new_bino;
    //cout<<"ft";
    for(vector<bino_node*> :: iterator it=my_heap.begin(); it!=my_heap.end(); it++)
    {
        //cout<<"tr";
        if(*it!= to_del)
        {
            new_bino.push_back(*it);
        }
    }
    //cout<<"et";
    //now we have to delete that element and make a new heap contains the children of deleted element
    //since children are also heaps(acc to property of bino heaps) we don't need to worry about looking at them
    vector<bino_node* > del_children;
    //go to one child and all it's siblings are binomial trees

    bino_node *chi=to_del->child;
    if(chi!=NULL)
    {
        chi->parent=NULL;
    }
    //cout<<"hi";
    bino_node *temp=chi;
    //cout<<"byr";
    while(chi!=NULL)
    {
        //cout<<"bruh";
        temp=chi;
        chi=chi->sib;
        temp->sib=NULL;
        temp->parent=NULL;
        del_children.push_back(temp);

    }
    reverse(del_children.begin(),del_children.end());
    delete to_del;
    return mergebinoheaps(new_bino,del_children);


}

bino_node* find_node(int label, bino_node* my_tree)
{
    if(my_tree==NULL)
    {
        return my_tree;
    }
    else
    {
        if(label==(my_tree->noder).second)
        {
            return my_tree;
        }
        else
        {
            if(find_node(label,my_tree->child)!=NULL)
            {
                return find_node(label,my_tree->child);
            }
            else
            {
                return find_node(label,my_tree->sib);
            }

        }

    }

}

vector<bino_node*> bino_deleteany(int label, vector<bino_node*>my_heap)
{
    //first find the node of the given label
    bino_node * mynode=NULL;
    for(vector<bino_node*> :: iterator it=my_heap.begin(); it!=my_heap.end(); it++)
    {
        bino_node *bino_tree=*it;
        if(((bino_tree->noder).second)==label)
        {
            mynode=bino_tree;
            break;
        }
        else
        {
            //cout<<"hiii"<<label<<endl;
            mynode=find_node(label,bino_tree->child);
            //cout<<"checking"<<endl;
            if(mynode!=NULL)
            {
                break;
            }
        }
    }
    if(mynode==NULL)
    {
        //cout<<"eee"<<endl;
        return my_heap;
    }
    else
    {
        //cout<<"hee"<<endl;
        //now if we find the desired node we will use decrease key operation to bring it to the top
        //and then use extract min
        (mynode->noder).first=-999999;
        bino_node *tempo=mynode->parent;
        
        while(tempo!=NULL)
        {
            if((tempo->noder).first >= (mynode->noder).first)
            {
                //cout<<"hiiiiiiiii"<<endl;
                //cout<<(mynode->noder).first<<" "<<(tempo->noder).first<<endl;
                swap(tempo->noder,mynode->noder);
                //cout<<(mynode->noder).first<<" "<<(tempo->noder).first<<endl;
                mynode=tempo;
                tempo=tempo->parent;
            }
            else
            {
                break;
            }

        }

        //now since -999999 is at the top and is min of all perform delete min
        //cout<<"yes"<<endl;
        my_heap=bino_deletemin(my_heap);
        //cout<<"working"<<endl;
        return my_heap;

    }

}

vector<int> binoheap_dijkastra(vector<vector<vector<int>>>w_adjlist,int source,vector<int>bell_dist)
{
    vector<int> distances(w_adjlist.size());
    vector<bino_node*>checker;                                         //binary heap
    //priority_queue<pair<int,int>>checker;                               //distance,label
    //cout<<"nahiiii";
    for(int i=1;i<w_adjlist.size();i++)
    {
        distances[i]=999999;
        //visited_or_not[i]=0;
    }
    int negatives=0;
    distances[source]=0;
    //cout<<"ho";
    checker=bino_insert(make_pair(0,source),checker);
    //cout<<"ho";
    //checker.push(make_pair(0,source));
    ////////////////////////////////////////////////////
    while(!checker.empty() && negatives==0)
    {

        //cout<<"hi";
        //int temp_source=checker[0].second;      //temp_source is the one with min distance in the heap we have to delete it now i.e perform extract min
        int temp_source=((find_min(checker))->noder).second;

        checker=bino_deletemin(checker);
        for(int i=0;i<w_adjlist[temp_source].size();i++)
        {
            //cout<<"hello ji";
            if(w_adjlist[temp_source][i][1]<0)
                {
                    negatives+=1;
                }
            //cout
            if(distances[w_adjlist[temp_source][i][0]]!=999999 || distances[temp_source]!=999999)
            {

                if(distances[temp_source]+w_adjlist[temp_source][i][1]<distances[w_adjlist[temp_source][i][0]])
                {
                    //cout<<w_adjlist[temp_source][i][0]<<"hellllll"<<endl;
                    //delete the node if found
                    //cout<<1;
                    checker=bino_deleteany((w_adjlist[temp_source][i][0]),checker);
                    //cout<<2;
                    //adjust distance in distance vector
                    distances[w_adjlist[temp_source][i][0]]=distances[temp_source]+w_adjlist[temp_source][i][1];
                    //cout<<3;
                    //insert again the value to be inserted
                    //checker.push_back(make_pair(distances[w_adjlist[temp_source][i][0]],w_adjlist[temp_source][i][0]));
                    checker=bino_insert(make_pair(distances[w_adjlist[temp_source][i][0]],w_adjlist[temp_source][i][0]),checker);
                    //cout<<4;
                }
            }
        }

        //cout<<"allset";
    }
    if(negatives!=0)
    {
        cout<<-1<<" ";
    }
    else
    {
        for(int i=1;i<distances.size();i++)
        {
            if(distances[i]!=999999)
            {
                distances[i]+=bell_dist[i]-bell_dist[source];
            }
            cout<<distances[i]<<" ";
        }
    }
    cout<<endl;
    return distances;
}


//fiboheaps

struct fibo_node
{
    pair<int,int>noder;            //distace,label
    int rank;
    int marked_or_not;         //0->not marked   1->marked
    struct fibo_node *parent;
    struct fibo_node *left;
    struct fibo_node *right;
    struct fibo_node *any_child;
};

struct fibo_node *fibomin=NULL;

void fibo_insert(pair<int,int>my_node)
{
    //struct fibo_node * node=(struct fibo_node *)malloc(sizeof(struct fibo_node));
    struct fibo_node * node= new fibo_node;
    node->noder=my_node;
    node->marked_or_not=0;
    //node->left=NULL;
    //node->right=NULL;
    node->left=node;
    node->right=node;
    node->any_child=NULL;
    node->parent=NULL;
    node->rank=0;

    if(fibomin==NULL)
    {
        fibomin=node;
    }
    else
    {
        if(fibomin->left==NULL || fibomin->right==NULL)
        {
            
            fibomin->left=node;
            node->right=fibomin;
            fibomin->left=node;
            node->left=fibomin;
            
        }
        else
        {
            (fibomin->left)->right=node;
            node->left=fibomin->left;
            node->right=fibomin;
            fibomin->left=node;
        }
        if(my_node.first<(fibomin->noder).first)
        {
            fibomin=node;
        }

    }

}

void fibo_extract_min()
{
    //if single node
    if(fibomin->left==NULL && fibomin->right==NULL)
    {
        fibomin=NULL;
        return;
    }
    else
    {   
        if(fibomin->left==fibomin && fibomin->any_child==NULL)
        {
            fibomin=NULL;
            return;
        }
        //merge all the chidren trees into the main linked list of heads 
        fibo_node* temp_min=fibomin;
        if(fibomin->any_child)
        {
            fibo_node *children=fibomin->any_child;
            fibo_node*temp=children->left;
            while(temp!=children)
            {
                temp->parent=NULL;
                temp=temp->left;
            }
        
            (fibomin->right)->left=children->left;
            (children->left)->right=fibomin->right;
            fibomin->right=children;
            children->left=fibomin;
            children->parent=NULL;
        }    
        //now remove the min 
        (temp_min->left)->right=temp_min->right;
        (temp_min->right)->left=temp_min->left;
        fibomin=temp_min->left;  //random value so that we don't loose the pointer
        temp_min->left=NULL;
        temp_min->right=NULL;
        temp_min->any_child=NULL;
        //free(temp_min);
        delete temp_min;
        //now we have the children of our deleted min in the main list
        //we need to reduce the size of main list by performing consolidation operation

        //this is a map that stores all nodes with a particular degree
        //map<int,fibo_node *>consi_map;
        vector <fibo_node *> consi_map(64,NULL);

        fibo_node* tem=fibomin;
        fibo_node* tem2=fibomin->right;
        consi_map[tem->rank]=tem;
        tem=tem->right;

        fibo_node *marker= fibomin; // this will keep a track if we have visited every node or not
        
        while(tem!=marker)
        {
            tem2=tem2->right;
            int checking_this_rank=tem->rank;
            while(consi_map[checking_this_rank]!=NULL)
            {
                struct fibo_node *match_with_this=consi_map[checking_this_rank];
                consi_map[checking_this_rank]=NULL;
                checking_this_rank+=1;
                //now we have to make sure that bigger label is linked to smaller one
                //we have to check this because we have not made sure if the main list is sorted or not
                if(((match_with_this)->noder).first < (tem->noder).first)
                {
                    fibo_node *tempa=tem;
                    tem=match_with_this;
                    match_with_this=tempa;
                }
                //now if we have that particular node gets combined we have to shift the marker to new position
                if(match_with_this== marker)
                {
                    marker=tem;
                }
                //now we have to combine the node with larger distance into the node with smaller distance
                //we have already made tem as the one with smaller distance and match_with_this as larger one

                //remove marke... from the main lis
                if(match_with_this!=tem)
                {
                    (match_with_this->left)->right= match_with_this->right;
                    (match_with_this->right)->left= match_with_this->left;
                    match_with_this->parent=tem;
                    if(!(tem->any_child))
                    {
                        tem->any_child=match_with_this;
                        match_with_this->right=match_with_this;
                        match_with_this->left= match_with_this;
                        tem->rank+=1;
                    }
                    else
                    {
                        match_with_this->left=((tem->any_child));
                        match_with_this->right=(tem->any_child)->right;
                        (((tem->any_child)->right)->left)=match_with_this;
                        (tem->any_child)->right=match_with_this;
                        tem->rank+=1;
                    }
                }
                //now after merging if only one node is left we have to make it the marker
                if(tem->left==tem)
                {
                    marker=tem;
                }

                
            }
            
            consi_map[checking_this_rank]=tem;
            tem=tem->right;
        
        }
        //now we have to find our min because it was lost/destroyed in the above process

        
        vector<fibo_node*>:: iterator itera=consi_map.begin();
        while(itera!=consi_map.end())
        {

            if(*itera)
            {
                if(fibomin==NULL)
                {
                    fibomin=*itera;
                }
                else
                {
                    if(((*itera)->noder).first < (fibomin->noder).first)
                    {
                        fibomin=*itera;
                    }

                }
                
            }

            itera++;
        }
        

        /*
        auto itera= consi_map.begin();
        while(itera!=consi_map.end())
        {

            if(itera->second)
            {
                if(fibomin==NULL)
                {
                    fibomin=itera->second;
                }
                else
                {
                    if(((itera->second)->noder).first < (fibomin->noder).first)
                    {
                        fibomin=itera->second;
                    }

                }
                
            }

            itera++;
        }
        */
        // using vector
        /*
        vector <fibo_node *> myarray(64,NULL);
        fibo_node *tem=fibomin;
        fibo_node *tem2=fibomin;
        myarray[tem->rank]=tem;
        fibo_node *marker= fibomin;

        while(tem!=marker)
        {
            tem=tem->right;
            if(myarray[tem->rank]==NULL)
            {
                myarray[tem->rank]=tem;
            }
            else
            {
                int check_this_rank=tem->rank;
                fibo_node *combine_this=myarray[tem->rank];
                if((combine_this->noder).first < (tem->noder).first)
                {
                    //make combine this as parent and tem its child
                    //but then ensure that marker is moved to right
                    //also ensure new tem is the marker

                    //to maintain uniformity just exchange bith the pointers here so that both cases can be handeled together
                    fibo_node *waster=tem;
                    tem=combine_this;
                    combine_this=waster;
                }
            }
            
        }

    */
    }
}

fibo_node * find_fibo(int label, fibo_node* start)
{
    if(start==NULL)
    {
        return NULL;
    }
    fibo_node *mark= start;
    //mark=start->right;
    do{
        if((mark->noder).second == label)
        {
            return mark;
        }
        else
        {
            fibo_node *temm=find_fibo(label, mark->any_child);
            if(temm!=NULL)
            {
                return temm;
            }
            else
            {
                mark=mark->right;
            }
            
        }
        
    }while(mark!=start);
    
    return NULL;
    
}

void fibo_deleteany(int label)
{
    if(fibomin==NULL)
    {
        return;
    }
    //first find the node
    fibo_node *found=find_fibo(label, fibomin);
    if(found==NULL)
    {
        return;
    }
    else
    {
        //now we have to cut this and send to the main list
        //also we have to unmark this
        //we have to mark parent if unmarked
        //if parent is marked cut it too and put in main list and so on...

        fibo_node *parr=found->parent;
        if(parr!=NULL)
        {
            if(found->left != found)
            {
                (found->parent)->any_child=found->left;
            }
            else
            {
                (found->parent)->any_child=NULL;
            }
            (found->parent)->rank--;
            //(found->parent)->marked_or_not=1;
            found->parent=NULL;
            (found->left)->right=found->right;
            (found->right)->left=found->left;
            found->marked_or_not=0;
            found->left=(fibomin->left);
            found->right=fibomin;
            (fibomin->left)->right=found;
            fibomin->left=found;
        }     
        if(parr!=NULL)
        {
            while(parr->parent!=NULL)
            {
                fibo_node *store_parent=parr->parent;
                if(parr->marked_or_not==0)
                {
                    parr->marked_or_not=1;
                    break;
                }
                else
                {
                    //now this is an already marked node so take it to the top
                    if(parr->left == parr)
                    {
                        (parr->parent)->any_child=NULL;
                    }
                    else
                    {
                        (parr->parent)->any_child=parr->left;
                    }
                    (parr->parent)->rank --;
                    ((parr->left)->right)=parr->right;
                    (parr->right)->left=parr->left;
                    parr->marked_or_not=0;
                    
                    store_parent=parr->parent;
                    
                    (parr->parent)->marked_or_not=1;
                    parr->parent=NULL;
                    parr->left=fibomin->left;
                    parr->right=fibomin;
                    (fibomin->left)->right=parr;
                    fibomin->left=parr;
                }
                

                parr=store_parent;
            }
            }
            (found->noder).first=-999999;
            fibomin=found;
            fibo_extract_min();
        
        
        
    }
    
   
}

vector<int> fibo_dijkastra(vector<vector<vector<int>>>w_adjlist,int source,vector<int>bell_dist)
{
    vector<int> distances(w_adjlist.size());
    //vector<pair<int,int>>checker;
    //priority_queue<pair<int,int>>checker;                               //distance,label
    
    for(int i=1;i<w_adjlist.size();i++)
    {
        distances[i]=999999;
        //visited_or_not[i]=0;
    }
    int negatives=0;
    distances[source]=0;
    fibo_insert(make_pair(0,source));
    //checker.push(make_pair(0,source));

    while(fibomin!=NULL && negatives==0)
    {
        //cout<<"hey";
        
        //int mini=checker[0].first;
        int temp_source=(fibomin->noder).second;
        fibo_extract_min();
        
        //checker.pop();
        for(int i=0;i<w_adjlist[temp_source].size();i++)
        {
            if(w_adjlist[temp_source][i][1]<0)
                {
                    negatives+=1;
                }
            //cout<<w_adjlist[temp_source][i][0]<<" "<<distances[w_adjlist[temp_source][i][0]]<<"bkjn";
            if(distances[w_adjlist[temp_source][i][0]]!=999999 || distances[temp_source]!=999999)
            {
                if(distances[temp_source]+w_adjlist[temp_source][i][1]<distances[w_adjlist[temp_source][i][0]])
                {
                    //if my destination is already in the set I need to update it and erase and insert again
                    //vector<pair<int,int>>::iterator it=find(checker.begin(),checker.end(),make_pair(distances[w_adjlist[temp_source][i][0]],w_adjlist[temp_source][i][1]));
                    //priority_queue<pair<int,int>>::iterator it=checker.find(make_pair(distances[w_adjlist[temp_source][i][0]],w_adjlist[temp_source][i][1]));
                    
                    fibo_deleteany(w_adjlist[temp_source][i][0]);
                    distances[w_adjlist[temp_source][i][0]]=distances[temp_source]+w_adjlist[temp_source][i][1];
                    fibo_insert(make_pair(distances[w_adjlist[temp_source][i][0]],w_adjlist[temp_source][i][0]));
                    //checker.push_back(make_pair(distances[w_adjlist[temp_source][i][0]],w_adjlist[temp_source][i][0]));
                    //checker.push(make_pair(distances[w_adjlist[temp_source][i][0]],w_adjlist[temp_source][i][0]));
                }
            }
        }
    }
    if(negatives!=0)
    {
        cout<<-1<<" ";
    }
    else
    {
        for(int i=1;i<distances.size();i++)
        {
            if(distances[i]!=999999)
            {
                distances[i]+=bell_dist[i]-bell_dist[source];
            }
            cout<<distances[i]<<" ";
        }
    }
    cout<<endl;
    return distances;
}


void main_function(int implement)
{
    int testcases;
    cin>>testcases;
    vector<double>comp_times;
    while(testcases--)
    {
        int n,d; //n->num of nodes, d->directed(1) or undirected(0)
        cin>>n>>d;
        vector<vector<int>> adj_mat;
        vector<vector<int>> orig_adj_mat;
        for(int i=0;i<n;i++)
        {
            vector<int>row;
            for(int j=0;j<n;j++)
            {
                int te;
                cin>>te;
                row.push_back(te);
            }
            orig_adj_mat.push_back(row);
            row.push_back(999999);
            adj_mat.push_back(row);
        }
        vector<int>wastevec(n,0);
        adj_mat.push_back(wastevec);
        
        vector<vector<vector<int>>>adj_list=convert_to_wadj_list(adj_mat);
        vector<vector<vector<int>>>orig_adj_list=convert_to_wadj_list(orig_adj_mat);

        vector<int>bellman_distances=modified_bellman(adj_list,n+1);
        start=clock();
        
        if(bellman_distances.size()==0)
        {
            cout<<-1<<endl;
        }
        else
        {

            
            for(int i=1;i<orig_adj_list.size();i++)
            {
                for(int j=0;j<orig_adj_list[i].size();j++)
                {
                    orig_adj_list[i][j][1]+=bellman_distances[i]-bellman_distances[orig_adj_list[i][j][0]];
                }
            }
            if(implement==1)
            {
                for(int i=1;i<n+1;i++)
                {
                    array_dijkastra(orig_adj_list,i,bellman_distances);
                }
            }
            else if(implement==2)
            {
                for(int i=1;i<n+1;i++)
                {
                    binaryheap_dijkastra(orig_adj_list,i,bellman_distances);
                }
            }
            else if(implement==3)
            {
                
                for(int i=1;i<n+1;i++)
                {
                    binoheap_dijkastra(orig_adj_list,i,bellman_distances);
                }
                
                //binoheap_dijkastra(orig_adj_list,3,bellman_distances);
            }
            else if(implement==4)
            {
                for(int i=1;i<n+1;i++)
                {
                    fibo_dijkastra(orig_adj_list,i,bellman_distances);
                }
            }
            /*
            for(int i=1;i<orig_adj_list.size();i++)
            {
                cout<<i<<" ";
                for(int j=0;j<orig_adj_list[i].size();j++)
                {
                    cout<<orig_adj_list[i][j][0]<<","<<orig_adj_list[i][j][1]<<"  ";
                }
                cout<<endl;
            }
            */

           /*
            for(int i=1;i<n+1;i++)
            {
                array_dijkastra(orig_adj_list,i,bellman_distances);
            }
            */

            //binary heap
            /*
            for(int i=1;i<n+1;i++)
            {
                binaryheap_dijkastra(orig_adj_list,i,bellman_distances);
            }
            */
            //binaryheap_dijkastra(orig_adj_list,2,bellman_distances);

           //binomial heap
           
           /*
           for(int i=1;i<n+1;i++)
           {
               binoheap_dijkastra(orig_adj_list,i,bellman_distances);
           }
           */
          //binoheap_dijkastra();
            
           //fiboheap
           
           /*
           for(int i=1;i<n+1;i++)
           {
               fibo_dijkastra(orig_adj_list,i,bellman_distances);
           }
           */
            //fibo_dijkastra(orig_adj_list,4,bellman_distances);
        }

        ende=clock(); 
        double timetaken=double(ende-start)/double(CLOCKS_PER_SEC);
        comp_times.push_back(timetaken);


    }
    for(int i=0;i<comp_times.size();i++)
    {
        cout << fixed  << comp_times[i] << setprecision(5)<<" ";
    }
    

}

/*
int main()
{
    int testcases;
    cin>>testcases;
    vector<double>comp_times;
    while(testcases--)
    {
        int n,d; //n->num of nodes, d->directed(1) or undirected(0)
        cin>>n>>d;
        vector<vector<int>> adj_mat;
        vector<vector<int>> orig_adj_mat;
        for(int i=0;i<n;i++)
        {
            vector<int>row;
            for(int j=0;j<n;j++)
            {
                int te;
                cin>>te;
                row.push_back(te);
            }
            orig_adj_mat.push_back(row);
            row.push_back(999999);
            adj_mat.push_back(row);
        }
        vector<int>wastevec(n,0);
        adj_mat.push_back(wastevec);
        
        vector<vector<vector<int>>>adj_list=convert_to_wadj_list(adj_mat);
        vector<vector<vector<int>>>orig_adj_list=convert_to_wadj_list(orig_adj_mat);

        vector<int>bellman_distances=modified_bellman(adj_list,n+1);
        start=clock();
        
        if(bellman_distances.size()==0)
        {
            cout<<-1<<endl;
        }
        else
        {

            
            for(int i=1;i<orig_adj_list.size();i++)
            {
                for(int j=0;j<orig_adj_list[i].size();j++)
                {
                    orig_adj_list[i][j][1]+=bellman_distances[i]-bellman_distances[orig_adj_list[i][j][0]];
                }
            }
            /*
            for(int i=1;i<orig_adj_list.size();i++)
            {
                cout<<i<<" ";
                for(int j=0;j<orig_adj_list[i].size();j++)
                {
                    cout<<orig_adj_list[i][j][0]<<","<<orig_adj_list[i][j][1]<<"  ";
                }
                cout<<endl;
            }
            */

           /*
            for(int i=1;i<n+1;i++)
            {
                array_dijkastra(orig_adj_list,i,bellman_distances);
            }
            */

            //binary heap
            /*
            for(int i=1;i<n+1;i++)
            {
                binaryheap_dijkastra(orig_adj_list,i,bellman_distances);
            }
            */
            //binaryheap_dijkastra(orig_adj_list,2,bellman_distances);

           //binomial heap
           
           /*
           for(int i=1;i<n+1;i++)
           {
               binoheap_dijkastra(orig_adj_list,i,bellman_distances);
           }
           */
          //binoheap_dijkastra();
            
           //fiboheap
           
           /*
           for(int i=1;i<n+1;i++)
           {
               fibo_dijkastra(orig_adj_list,i,bellman_distances);
           }
           */
            //fibo_dijkastra(orig_adj_list,4,bellman_distances);
            
          /*  
        }

        ende=clock(); 
        double timetaken=double(ende-start)/double(CLOCKS_PER_SEC);
        comp_times.push_back(timetaken);


    }
    for(int i=0;i<comp_times.size();i++)
    {
        cout << fixed  << comp_times[i] << setprecision(5)<<" ";
    }
    
}
*/
int main(int argc, char ** argv)
{
    int implement;
    if(argc==1)
    {
        implement=4;
    }
    else
    {
        if(*argv[1]=='1')
        {
            implement=1;
        }
        else if(*argv[1]=='2')
        {
            implement=2;
        }
        else if(*argv[1]=='3')
        {
            implement=3;
        }
        else if(*argv[1]=='4')
        {
            implement=4;
        }
    }

    main_function(implement);
    
}