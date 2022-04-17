#include<stdio.h>
#include<stdlib.h>
#define MAX_SIZE 999//size of heap using is 0-999 i.e 1000 size

typedef enum{false,true} bool;//typedef for boolean type
typedef struct List_node_tag
{
	int start;
	int end;
	char name[50];
	struct List_node_tag* next;
	struct List_node_tag* prev;
	int is_Free;
} List_node;//double linked list node structure

typedef struct heap_tag
{
	List_node* lptr;
} heap;//heap structure

void strcopy(char* s1,char* s2)//funtion for copying one string to other i.e strcpy function source code
{
	int i=0;
	while(s2[i]!='\0')
	{
		s1[i]=s2[i];
		i++;
	}
	s1[i]='\0';
}
 
int strcompare(char s1[],char s2[])//function for checking if two strings are same or not i.e strcmp function source code
{
   int i=0;
   while(s1[i]!='\0')
   {
   	if(s2[i]=='\0')
   	   return 1;
   	else if(s1[i]<s2[i])
   	   return -1;
   	else if(s1[i]>s2[i])
   	   return 1;
   	i++;
   }
   return 0;
}

List_node* CreateAllocatedNode(int start,int end,char name[])//for allocating required space for a variable
{
    List_node* ptr = (List_node*)malloc(sizeof(List_node));//for creating a node in heap for that variable
    if(ptr!=NULL) 
	{
        ptr->start = start;
    	ptr->end = end;
    	strcopy(ptr->name,name);//copying given name in that node created 
    	ptr->next = NULL;
        ptr->prev = NULL;
        ptr->is_Free = 0;//as this node is occupied now we cant further allocate until its again made free using FreeSpace func
    }
	return ptr;
}

List_node* CreateFreeNode(int start, int end)//for creating a free node that can be further used for allocation
{
    List_node* ptr= (List_node*)malloc(sizeof(List_node));
    if(ptr!=NULL) {
        ptr->start = start;
        ptr->end = end;
        ptr->next = NULL;
        ptr->prev = NULL;
        ptr->is_Free = 1;//as free node is created now we can use it for further allocation so its made free
    }
    return ptr;
}

bool isListFull(heap* h)//checks if the list created for heap is full or not
{
    bool res = true;
    List_node* ptr = h->lptr;
    while(ptr!=NULL && !(ptr->is_Free))//this runs until a free node is found or ptr reaches NULL
	{
        ptr = ptr->next;
    }
    if(ptr!=NULL)//if the above loop ended because of finding a free block then it means list is not full
        res=false;
    return res;
}

bool isListEmpty(heap *h)//checks if the list created for heap is empty or not
{
    int res = true;
    List_node* ptr = h->lptr;
    while(ptr!=NULL && ptr->is_Free) //this runs until a allocated node is found or ptr reaches NULL
	{
        ptr = ptr->next;
    }
    if(ptr!=NULL)//if the above loop ended because of finding a allocated block then it means list is not empty
        res=false;
    return res;
}

bool isAlreadyThere(heap *h, char name[])//checks if a variable name created already existing or not
{
    List_node* lptr = h->lptr;
    bool res = false;
	int val;
    while(lptr!=NULL) 
	{
		val=strcompare(name,lptr->name);//using strcompare func to check if that variable name given already there or not
        if(!(lptr->is_Free) && val==0)//checking that new variable name created that needs heap space for allocation already existing or not
        {
            res = true;
        }
        lptr = lptr->next;
    }
    return res;
}

void allocateFirstFit(heap *h, int size, char name[])//using firstfit allocation for requested heap space if available
{
    List_node* lptr = h->lptr;
    if(lptr == NULL) 
	{
        printf("\nNo free list for heap is created yet!!!!\n");//error message if free block list for heap is not created
    }
    else 
	{
        List_node* nptr = lptr;
		List_node* ptr = NULL;
        int done = 0;
        while(nptr!=NULL && !done) //loop runs until nptr reaches NULL or required space for allocation is found
		{
            if(nptr->is_Free && (nptr->end-nptr->start+1) >= size) //checks if a block is free and has enough space for required allocation
			{
                ptr = nptr;
                done = 1;
            }
            nptr = nptr->next;
        }
        int end;
        if(ptr!=NULL) //it means free space requried is found before ptr reaches NULL
		   end = ptr->start + size -1;
        if(ptr == NULL || ( ptr->next==NULL && (end > MAX_SIZE) ) ) //if the ptr reached NULL or if the free space present not enough for this alllocation
		{
            printf("\nThis much free space is not available!!!!\n");
        }
        else//Means free space available is enough for the allocation
		{
            List_node* aptr = CreateAllocatedNode(ptr->start,end,name);
            if(aptr==NULL) //if allocation fails
			{
                printf("\nAllocation failure!!!\n");
            }
            else //Free space available is getting allocated 
			{
                if(end == ptr->end) //if a node end is same as required block end then just making allocation i.e entire free block getting allocated
				{
                    lptr = aptr;
                    if(ptr->prev!=NULL) 
					{
                        ptr->prev->next = lptr;
                        lptr->prev = ptr->prev;
                    }
                    if(ptr->next!=NULL) 
					{
                        ptr->next->prev = lptr;
                        lptr->next = ptr->next;
                    }
                    free(ptr);
                }
                else //if only some part of the free block getting allocated
				{
                    ptr->start = end+1;
                    lptr = aptr;
                    if(ptr->prev!=NULL) 
					{
                        ptr->prev->next = lptr;
                        lptr->prev = ptr->prev;
                    }
                    lptr->next = ptr;
                    ptr->prev = lptr;
                }
                printf("\nSpace allocation successful!!!\n");
                while(lptr->prev != NULL) //getting back our heap lptr
				{
                    lptr = lptr->prev;
                }
                h->lptr = lptr;
            }
        }
    }
}

void allocateBestFit(heap *h, int size, char name[])//using bestfit allocation for requested heap space if available
{
    List_node* lptr = h->lptr;
    if(lptr == NULL) 
	{
        printf("\nNo free list for heap is created yet!!!!\n");//error message if free block list for heap is not created
    }
    else 
	{
        List_node* nptr = lptr;
		List_node* ptr = NULL;
        int Size1=1001;
        while(nptr!=NULL) //loop runs until nptr reaches NULL for best fit block search
		{
            if(nptr->is_Free && (nptr->end-nptr->start+1) >= size) //checks if a block is free and has enough space for required allocation
			{
                if((nptr->end-nptr->start+1)<Size1)//check for the best fit block
                {
                	Size1=(nptr->end-nptr->start+1);
                	ptr=nptr;
				}
            }
            nptr = nptr->next;
        }
        int end;
        if(ptr!=NULL) //it means free space requried is found before ptr reaching NULL
		   end = ptr->start + size -1;
        if(ptr == NULL || ( ptr->next==NULL && (end > MAX_SIZE) ) ) //if the ptr reached NULL or if the free space present not enough for this allocation
		{
            printf("\nThis much free space is not available!!!!\n");
        }
        else//Means free space available is enough for the allocation
		{
            List_node* aptr = CreateAllocatedNode(ptr->start,end,name);
            if(aptr==NULL) //if allocation fails
			{
                printf("\nAllocation failure!!!\n");
            }
            else //Free space available is getting allocated 
			{
                if(end == ptr->end) //if a node end is same as required block end then just making allocation i.e entire free block getting allocated
				{
                    lptr = aptr;
                    if(ptr->prev!=NULL) 
					{
                        ptr->prev->next = lptr;
                        lptr->prev = ptr->prev;
                    }
                    if(ptr->next!=NULL) 
					{
                        ptr->next->prev = lptr;
                        lptr->next = ptr->next;
                    }
                    free(ptr);
                }
                else //if only some part of the free block getting allocated
				{
                    ptr->start = end+1;
                    lptr = aptr;
                    if(ptr->prev!=NULL) 
					{
                        ptr->prev->next = lptr;
                        lptr->prev = ptr->prev;
                    }
                    lptr->next = ptr;
                    ptr->prev = lptr;
                }
                printf("\nSpace allocation successful!!!\n");
                while(lptr->prev != NULL) //getting back our heap lptr
				{
                    lptr = lptr->prev;
                }
                h->lptr = lptr;
            }
        }
    }
}

void FreeSpace(heap *h, char name[])//freeing up space that is allocated to a variable
{
    List_node* lptr = h->lptr;
	List_node* ptr = NULL;
    if(isListEmpty(h)) //check for if the list is empty then nothing can be freed up further
	{
        printf("\nNo space is allocated yet!!!\n");
    }
    else//if the list is not empty then it will check for given variable name
	{
        int found=0,val=-2;
        while (lptr!=NULL && !found)//loop runs until the variable name given is found in the list or the lptr reaches NULL 
		{
			val=strcompare(name,lptr->name);//call for function that compares two strings whether they are equal or not
            if(!(lptr->is_Free) && val==0) 
			{
                ptr = lptr;
                found = 1;
            }
            lptr = lptr->next;
        }
        if(ptr==NULL) //if ptr is NULL it means variable with given name is not existing in the list
		{
            printf("\nThere is no such variable name so freeing up space not done!!!!!\n");
        }
        else//if the variable that need to be deallocated is found 
		{
            List_node* aptr;
            ptr->is_Free = 1;//making that block available for further allocation
            if(ptr->prev!=NULL && ptr->prev->is_Free) //cheching for allocated block start point
			{
                aptr = ptr->prev;
                ptr->start = aptr->start;
                if(aptr->prev != NULL) 
				  aptr->prev->next = ptr;
                ptr->prev = aptr->prev;
                free(aptr);
            }
            if(ptr->next!=NULL && ptr->next->is_Free) //checking for allocated block end point
			{
                aptr = ptr->next;
                ptr->end = aptr->end;
                if(aptr->next != NULL) 
				  aptr->next->prev = ptr;
                ptr->next = aptr->next;
                free(aptr);
            }
            lptr = ptr;
            while(lptr->prev != NULL)//getting back to the heap lptr
			{
                lptr = lptr->prev;
            }
            printf("\nFreeing up space successful!!!\n");
            h->lptr = lptr;
        }
    }
}

void displayAllocatedList(heap *h)//for printing out the allocated blocks in the list until now
{
    if(isListEmpty(h))//if none of the blocks are allocated list will be emoty and nothing to show as allocated blocks 
	{
        printf("\nNo space in heap is allocated to show!!!\n");
    }
    else //displays all variables that were provided with space on heap along with their size and starting and ending blocks of allocation
	{
        List_node* ptr = h->lptr;
        printf("\nVariable\tStarting\tEnding\t\tSize\n");
		while(ptr!=NULL) 
		{
			if(!(ptr->is_Free))//checking which blocks are allocated using this
				printf("%s\t\t%d\t\t%d\t\t%d\n",ptr->name,ptr->start,ptr->end,ptr->end-ptr->start+1);
			ptr = ptr->next;
		}
    }
}

void displayFreeList(heap *h)//for printing out all he free blocks available for allocation along with their sizes
{
    if(isListFull(h))//if the list created for heap is full then nothing to show as free blocks
	{
        printf("\nNo space in heap is free to show!!!\n");
    }
    else //displays all the free blocks available along with their starting and ending blocks and respective sizes
	{
        List_node* ptr = h->lptr;
        printf("\nStarting\tEnding\t\tSize\n");
		while(ptr!=NULL) 
		{
			if(ptr->is_Free)//check for free blocks
				printf("%d\t\t%d\t\t%d\n", ptr->start, ptr->end, (ptr->end)-(ptr->start)+1);
			ptr = ptr->next;
		}
    }
}

void displayHeap(heap *h)//prints out all the allocated and all the available free blocks
{
    printf("\nAllocated blocks are :\n");
    displayAllocatedList(h);//call for the func that displays all allocated blocks for the variables
    printf("\nFree blocks are :\n");
    displayFreeList(h);//call for the function that displays all free blocks available in the heap
    printf("\n");
}

void deleteList(heap *h)//deletes whole list that has been created for heap implementation
{
    List_node* ptr;
    ptr = h->lptr;
    while(h->lptr!=NULL)
	{
        ptr = h->lptr;
        h->lptr = h->lptr->next;
        free(ptr);
    }
    //h->lptr = NULL;
}

int main()
{
    heap myheap;
    heap* ptr = &myheap;
	List_node* nptr = CreateFreeNode(0,MAX_SIZE);//creating a required amount of list for heap
    if(nptr == NULL) 
	{
        printf("\nThis much space is pretty large for free list creation\n");
    }
    else 
	{
        ptr->lptr = nptr;
    	int exit=0,select,size=0;
    	char name[50];
        printf("Total heap space available is %d\n",1+(nptr->end)-(nptr->start));//displaying whole space on heap available
    	while(exit==0) {
    		printf("\nEnter the value:\n1 to Allocate space using Bestfit\n2 to Allocate space using Firstfit\n3 to Free space\n4 to display whole heap\n5 to display allocated blocks\n6 to display available free blocks\n0 to exit\n");
    		printf("\nEnter value:");//input from user about which operation need to be implemented
			scanf("%d", &select);
			int flag=0;
            switch(select) 
			{
				case 1 ://to allocate space using bestfit strategy
				{ 
				printf("\nEnter variable name:");//taking input of variable name for which we do heap allocation
				scanf("%s",name);
				printf("Enter the size:");//variable size
                scanf("%d",&size);
                if(isListFull(ptr))//check if list is already full
                    printf("\nHeap is completely allocated!!!\n");
                else if(isAlreadyThere(ptr,name))//the varibale name entered is already present then avoid allocation for it
                    printf("\nVariable name already existing.Please choose another!!!\n");
                else
                    allocateBestFit(ptr,size,name);//call for the function to allocate space for a varaible where its size is given
                flag=1;
                }
                case 2 ://to allocate space using firstfit strategy
				{ 
				if(flag==0)
				 {
				   printf("\nEnter variable name:");//taking input of variable name for which we do heap allocation
				   scanf("%s",name);
				   printf("Enter the size:");//variable size
                   scanf("%d",&size);
                   if(isListFull(ptr))//check if list is already full
                     printf("\nHeap is completely allocated!!!\n");
                   else if(isAlreadyThere(ptr,name))//the variable name entered is already present then avoid allocation for it
                     printf("\nVariable name already existing.Please choose another!!!\n");
                   else
                     allocateFirstFit(ptr,size,name);//call for the function to allocate space for a varaible where its size is given
                   flag=1;
                 }
                }
                case 3 : //to freeup space that is by deleting space allocated for a variable
				{
				  if(flag==0)
				  {
				  printf("\nEnter variable name:");	//input for variable  which we are deleting
				  scanf("%s",name);
                  FreeSpace(ptr,name);//call for the function to free the space that is allocated for this variable
                  }
                  flag=1;
                }
                case 4 : //to display entire heap allocated and free blocks
				{
				   if(flag==0)
		             displayHeap(ptr);//function call to print out all the allocated and all the available free blocks
                   flag=1;
                }
                case 5 : //to display all allocated blocks in the list
				{
				   if(flag==0)
				    displayAllocatedList(ptr);//function call to print out the allocated blocks in the list until now
                   flag=1;
                }
                case 6 : //to display all free blocks available in the list
				{
				  if(flag==0)
			        displayFreeList(ptr);//function call to print out all he free blocks available for allocation along with their sizes
                  flag=1;
                }
                case 0 : //to exit the process 
				{
				   if(flag==0)
				   {
			         printf("\nExited successfully!!!\n");
                     exit=1;
                   }
                }
            }
    	}
        deleteList(ptr);//deleting entire list created for heap after doing implementation
    }
    return 0;
}
