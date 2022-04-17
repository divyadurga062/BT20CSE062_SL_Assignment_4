#include<stdio.h>
#include<stdbool.h>
#include<stdlib.h>

typedef struct node
{
    int data;//value
    bool mark_bit;//for marking the nodes
    int ref_count;//reference count mechanism
    struct node *next_1;
    struct node *next_2;
    struct node *next_3;
}Node;

 Node *pointer_array[8];

void display_node(int i)
{
	printf("Data value=%d\t reference count=%d freed size=%d\n",pointer_array[i]->data,pointer_array[i]->ref_count,sizeof(Node));
}//function to display reference count and freed up size

void set_edges(int s,int d1,int d2,int d3)
{
	if(d1!=-1)
	{
		pointer_array[s]->next_1=pointer_array[d1];
		pointer_array[d1]->ref_count+=1;
	}
	if(d2!=-1)
	{
		pointer_array[s]->next_2=pointer_array[d2];
		pointer_array[d2]->ref_count+=1;
	}
	if(d3!=-1)
	{
		pointer_array[s]->next_3=pointer_array[d3];
		pointer_array[d3]->ref_count+=1;
	}
	
}

void display_all_nodes(Node* root)
{
	if(root!=NULL)
	{
		printf("Data value=%d:reference count=%d\n",root->data,root->ref_count);
	}
	if(root==NULL)
	{
		return;
	}
	display_all_nodes(root->next_1);
	display_all_nodes(root->next_2);
	display_all_nodes(root->next_3);
}

void adjacency_list()
{
	int i=0;
	for(i=0;i<8;i++)
	{
		if(pointer_array[i]!=NULL)
		{
			printf("Value=%d: ",pointer_array[i]->data);
			if(pointer_array[i]->next_1!=NULL)
			{
				printf("%d ->",pointer_array[i]->next_1->data);
			}
			if(pointer_array[i]->next_2!=NULL)
			{
				printf("%d ->",pointer_array[i]->next_2->data);
			}
			if(pointer_array[i]->next_3!=NULL)
			{
				printf("%d ->",pointer_array[i]->next_3->data);
			}
			printf("NULL\n");
		}
	}
}

int root_is_present(Node* root1,Node* temp)
{
	if(root1==NULL)
	{
		return 0;
	}
	if(root1->data==temp->data)
	{
		return 1;
	}
	
	if(root_is_present(root1->next_1,temp))
	{
		return 1;
	}
	
	if(root_is_present(root1->next_2,temp))
	{
		return 1;
	}
	if(root_is_present(root1->next_3,temp))
	{
		return 1;
	}
 return 0;
}


void garbage_collection_reference_counting(Node* root)
{
	int i=0;
	while(i<8)
	{
		if(root_is_present(root,pointer_array[i])==0 )
		{		
			if(pointer_array[i]->next_1!=NULL)
			{
				pointer_array[i]->next_1->ref_count-=1;
			}
			if(pointer_array[i]->next_2!=NULL)
			{
				pointer_array[i]->next_2->ref_count-=1;
			}
			if(pointer_array[i]->next_3!=NULL)
			{
				pointer_array[i]->next_3->ref_count-=1;
			}
			printf("Garbage identified:");
			display_node(i);
			free(pointer_array[i]);
			pointer_array[i]=NULL;
		}
		 i++;		
	}
	 
}

void adjacency_Matrix()
{
	int ad[8][8];
	int i,j,k;
	
	for(i=0;i<8;i++)//initialising the matrix
	{
		for(j=0;j<8;j++)
		{
			ad[i][j]=0;
		}	
	}
	
	for(i=0;i<8;i++)
	{
		for(j=0;j<8;j++)
		{
			
		if(pointer_array[j]!=NULL&&pointer_array[i]!=NULL)
		{
			
			if(pointer_array[i]->next_1!=NULL)
			{
				if(pointer_array[i]->next_1->data==pointer_array[j]->data&&i!=j)
				{
					ad[i][j]=1;
				}
			}
			if(pointer_array[i]->next_2!=NULL)
			{
				if(pointer_array[i]->next_2->data==pointer_array[j]->data&&i!=j)
				{
					ad[i][j]=1;
				}
			}
			if(pointer_array[i]->next_3!=NULL)
			{
				if(pointer_array[i]->next_3->data==pointer_array[j]->data&&i!=j)
				{
					ad[i][j]=1;
				}
			}
		}
		
		}
	}
	
	for(i=0;i<8;i++)//printing the adjacency matrix
	{
		for(j=0;j<8;j++)
		{
			printf("%d ",ad[i][j]);
		}
		printf("\n");
	}
}

void marking_the_Nodes(Node*root,int i,int j)
{
    Node *current, *pre;

    current = root;
        
    while (current != NULL) 
    {
  
        if (current->next_1== NULL) 
        {
            current->mark_bit=true;
            current = current->next_2;
        }   
        else 
        {
            pre = current->next_1;
            while ((pre->next_2 != NULL) && (pre->next_2 != current))
            {
            	pre = pre->next_2;
			}
                
            if (pre->next_2 == NULL) 
            {
                pre->next_2 = current;
                current = current->next_1;
            }
            else 
            {
                pre->next_2 = NULL;
                current->mark_bit=true;
                current = current->next_2;
            } 
        }
    }   
    
    current = root;
        
    while (current != NULL) 
    {
  
        if (current->next_1== NULL) 
        {
            current->mark_bit=true;
            current = current->next_3;
        }   
        else 
        {
            pre = current->next_1;
            while ((pre->next_3 != NULL) && (pre->next_3 != current))
            {
            	pre = pre->next_3;
			}
                
            if (pre->next_3 == NULL) 
            {
                pre->next_3 = current;
                current = current->next_1;
            }
            else 
            {
                pre->next_3 = NULL;
                current->mark_bit=true;
                current = current->next_3;
            } 
        }
    }  
    
}

void marking_method(Node* root)
{
	
	if(root!=NULL)
	{
		root->mark_bit=true;
	}
	if(root==NULL)
	{
		return;
	}
	marking_method(root->next_1);
	marking_method(root->next_2);
	marking_method(root->next_3);
}

void sweeping_method()
{
	int i;
	for(i=0;i<8;i++)
	{
		if(pointer_array[i]->mark_bit==false)
		{
			if(pointer_array[i]->next_1!=NULL)
			{
				pointer_array[i]->next_1->ref_count-=1;
			}
			if(pointer_array[i]->next_2!=NULL)
			{
				pointer_array[i]->next_2->ref_count-=1;
			}
			if(pointer_array[i]->next_3!=NULL)
			{
				pointer_array[i]->next_3->ref_count-=1;
			}
			printf("Garbage identified :");
			display_node(i);
			free(pointer_array[i]);
			pointer_array[i]=NULL;
		}
	}
}

int main()
{
	
	int val[]={1,2,3,5,7,8,9,10};
	int i;
	
	for( i=0;i<8;i++)
	{
		Node* newNode =(Node*)malloc(sizeof(Node));
		newNode->data=val[i];
		newNode->next_1=NULL;
		newNode->next_2=NULL;
		newNode->next_3=NULL;
		newNode->ref_count=0;
		newNode->mark_bit=false;		
		pointer_array[i]=newNode;
	}
	
	Node*root1=pointer_array[3];
	pointer_array[3]->ref_count+=1;
	Node*root2=pointer_array[0];
	pointer_array[0]->ref_count+=1;
	
	set_edges(0,1,6,7);
	set_edges(2,5,7,-1);
	set_edges(3,0,-1,-1);
	set_edges(4,0,5,-1);
	set_edges(5,6,-1,-1);
	
	printf("\nDisplay of nodes through Root-1:\n\n");
	display_all_nodes(root1);
	
		
	printf("\nDisplay of nodes through Root-2:\n\n");
	display_all_nodes(root2);
	
	printf("\nAdjacency list :\n\n");//Display of Adjacency list of the nodes
	adjacency_list();
	
	printf("\nAdjacency matrix:\n\n");//Display of Adjacency Matrix of the all nodes
	adjacency_Matrix();
	
	printf("\nCalling the mark-sweep garbage collector\n");
	marking_method(root1);
	sweeping_method();

	printf("\nAdjacency list after garbage removal:\n\n");
	adjacency_list();
	
	printf("\nAdjacency matrix after garbage removal:\n\n");
	adjacency_Matrix();
	
}
