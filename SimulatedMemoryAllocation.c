#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <limits.h>

#define SIZE 20

struct Node {
    struct Node *prev;
    struct Node *next;
    long int start;
    long int length;
    char Nname[SIZE];
};

void Request(char _name[], long int _length, char _type, struct Node **head);

struct Node* Best(char _name[], long int _length, struct Node *head);

struct Node* Worst(char _name[], long int _length, struct Node *head);

struct Node* First(char _name[], long int _length, struct Node *head);

void Release(char _name[], struct Node **head);

void Status(struct Node *head);

void Compaction(struct Node **head);

int main(int argc, char *argv[] ) {

    char command[SIZE];
    char name[SIZE] = "Unused";
    long int length = atoi(argv[1]) - 1;
    long int available = atoi(argv[1]) - 1;
    char fit[SIZE];
    int finished = 0;


    struct Node *head;
    head = (struct Node*) malloc(sizeof(struct Node));

    head->prev = NULL;
    head->next = NULL;
    head->start = 0;
    head->length = available;
    int test = 1;
    strcpy(head->Nname, "Unused");

    printf("Options: RQ (request), RL (release), C (compact), STAT (status report), and X (exit)\n");
    printf("Input should be in the format: Option AllocationName AllocationSize(Bytes) FitType\n");
    printf("Fit Types: F (first fit), B (best fit), W (worst fit)\n");
    printf("e.g. RQ P0 40000 W\n");
    
    if(test == 1)
    {
        while(!finished) {
            printf("allocator>");

            scanf("%s", command);
            if (strcmp(command,"X")==0) {
                printf("Exiting program.\n");
                finished = 1;
            } else if (strcmp(command,"C")==0) {
                Compaction(&head);
            } else if (strcmp(command,"STAT")==0) {
                Status(head);
            } else if (strcmp(command,"RL")==0) {
                scanf("%s", name);
                Release(name, &head);
            } else if(strcmp(command,"RQ")==0) {
                scanf("%s%li%s", name, &length, fit);
                //validate that the request is not too large
                Request(name, length, *fit, &head);
                //commandRQ(name, length, fit);
            }
            else
            {
                //not valid command
            }
        
            //printf("Command:%s|Name:%s|Size:%li|Fit:%s", command, name, length, fit);
            //printf("\n");   

            command[0] = '\0';
            name[0] = '\0';
            length = 0;
            fit[0] = '\0';

            printf("\n");

        }
    }
    //test code
    else{
        Request("P0", 100, 'W', &head);
        Request("P1", 100, 'F', &head);
        Request("P2", 100, 'F', &head);
        Request("P3", 100, 'F', &head);
        Request("P4", 200, 'W', &head);
        Request("P5", 100, 'F', &head);
        Request("P6", 100, 'F', &head);
        Request("P7", 100, 'F', &head);
        Request("P8", 100, 'F', &head);

        Status(head);
        printf("===============================================\n");

        Release("P8", &head);
        Status(head);
        printf("===============================================\n");

        Release("P2", &head);
        Status(head);
        printf("===============================================\n");

        Release("P4", &head);
        Status(head);
        printf("===============================================\n");

        Release("P0", &head);
        Status(head);
        printf("===============================================\n");

        // Request("P8", 100, 'B', &head);
        // Request("P0", 100, 'W', &head);
        // Request("P10", 1000, 'B', &head);
        Compaction(&head);

        Release("P5", &head);
        Status(head);
        printf("===============================================\n");

        Release("P8", &head);
        Status(head);
        printf("===============================================\n");

        Release("P6", &head);
        Status(head);
        printf("===============================================\n");

        Release("P4", &head);
        Status(head);
        printf("===============================================\n");

        Release("P7", &head);
        Status(head);
        printf("===============================================\n");

        Release("P10", &head);
        Status(head);
        printf("===============================================\n");

        Release("P1", &head);
        Status(head);
        printf("===============================================\n");

        Release("P0", &head);
        Status(head);
        printf("===============================================\n");
        
        Compaction(&head);
        Status(head);
    }
    
    return 0;
}


void Compaction(struct Node **head)
{
    printf("in compaction\n");
    //NOTE: There can never be two adjacent Unused Nodes, there must always be at least one used Node between Unused Nodes
    struct Node *rover = (*head);
    struct Node *swapper = NULL;
    struct Node *temp = NULL;
    long int tempNum = 0;
    int found = 0;

    //find an Unused for rover
    while(rover != NULL && found == 0)
    {
        if(strcmp(rover->Nname, "Unused") == 0)
            found = 1; //time to go onto the next step
        else
            rover = rover->next;
    }
    
    //push Unused forward untill rover->next == NULL merging Unused as you go
    while(rover->next != NULL)
    {
        //rover from now on is Unused, as such we either swap rover and rover->next or merge them
        if(strcmp(rover->next->Nname, "Unused") == 0)//que to merge the two nodes
        {
            temp = rover->next;

            rover->length += temp->length + 1;

            if(temp->next != NULL)
            {
                temp->next->prev = rover;
            }
            
            rover->next = temp->next;

            free(temp);
        }
        else
        {
            //swapping acts as a sort of iterator becuase eventually rover->next will be NULL
            swapper = rover->next;

            if(rover->prev != NULL)
                rover->prev->next = swapper;

            if(swapper->next != NULL)
                swapper->next->prev = rover;

            tempNum = rover->start;

            //swaping starts is based on legths
            rover->start = swapper->start - (rover->length - swapper->length);
            swapper->start = tempNum;


            if(tempNum == 0)
                (*head) = swapper;

            rover->next = swapper->next;
            swapper->prev = rover->prev;

            rover->prev = swapper;
            swapper->next = rover;
        }
    }
}

void Status(struct Node *head)
{
    struct Node *rover = head;

    while (NULL != rover && NULL != rover->next)
    {
        printf("Addresses [%ld:%ld] Process %s\n", rover->start, (rover->start + rover->length), rover->Nname);
            
        rover = rover->next;
    }
    
    printf("Addresses [%ld:%ld] Process %s\n", rover->start, (rover->start + rover->length), rover->Nname);
            
    //possibly print one more
}

void Request(char _name[], long int _length, char _type, struct Node **head)
{
    
    struct Node *add = (struct Node*) malloc(sizeof(struct Node));
    struct Node *curr = NULL;
    add->length = _length - 1;
    strcpy(add->Nname, _name);

    
    //If you get Null try compaction, then try again

    if(_type == 'F')
    {
        curr = First(_name, _length, *head);
    }
    else if(_type == 'B')
    {
        curr = Best(_name, _length, *head);
    }
    else if (_type == 'W')
    {
        curr = Worst(_name, _length, *head);
    }
    else
        return;

    //determain if temp and head are in the same spot(*head)->length >= add->length
    if(curr->start == 0 && strcmp((*head)->Nname, "Unused") == 0)
    {
        curr = (*head); //lets changes to the "Unused" head without needing head
        (*head) = add; //sets the list up for changing heads

        add->prev = curr->prev;
        add->next = curr; //both of add pointers
        curr->prev = add; //curr->next is already where it's supposed to be
        add->start = 0;

        //change the lengths of stuff after the new head;
        add->next->length -= add->length + 1;
        add->next->start += add->length;
    }
    else//all other cases
    {
        add->start = curr->length + curr->start + 1; //start at the end of the one before the empty space

        if(curr->next != NULL)
        {
            //make changes to curr->next size
            curr->next->start += add->length;
            curr->next->length -= add->length;
        }

        //fix add pointers
        add->next = curr->next;
        add->prev = curr;

        curr->next = add;
        if(add->next != NULL)
        {
            //change curr->next and curr
            add->next->prev = add;
            add->next->length -= 1;
        }
    }
    if(add->next != NULL)
        add->next->start += 1;
}

struct Node* First(char _name[], long int _length, struct Node *head)
{
    struct Node *rover = head; // Rover points to the same node in the linked list as head
    struct Node *ret = NULL;

    if(strcmp(head->Nname, "Unused") == 0 && head->length >= _length)// make sure it's not the head
        ret = head;

    while (rover->next != NULL && ret == NULL) // while rover position is not the last node in the linked list
    {
        if(strcmp(rover->next->Nname, "Unused") == 0 && _length < rover->next->length)
        {
            // Nname = Node name , if the next node is "unused" && process length is less than the length of "unused"
            ret = rover; //return the node before "unused"
        }

        rover = rover->next; // only does this if it's not unused, moves from first node to next node
    }

    return ret;
}

struct Node* Best(char _name[], long int _length, struct Node *head)
{
    struct Node *rover = head;
    struct Node *ret = NULL;
    long int curDifference = 9223372036854775806; //long max

    if(strcmp(head->Nname, "Unused") == 0 && head->length >= _length && (head->length - _length) <= curDifference)
    {
        ret = head;
        curDifference = head->length - _length;
    }

    while(rover->next != NULL)
    {
        if(strcmp(rover->next->Nname, "Unused") == 0 && rover->next->length >= _length && rover->next->length - _length <= curDifference)
        {
            ret = rover;
            curDifference = rover->next->length - _length;
        }
        rover = rover->next;
    }

    return ret;
}

struct Node* Worst(char _name[], long int _length, struct Node *head)
{
    struct Node *rover = head; // Rover points to the same node in the linked list as head
    struct Node *ret = NULL;
    long int differnce = -1;

    if(strcmp(head->Nname, "Unused") == 0 && head->length >= _length)// if the first node was "unused" and it is large enough to hold the process
    {
        ret = head;
        differnce = head->length - _length;
    }

    while (rover->next != NULL) // while rover position is not the last node in the linked list, and the entire memory was not open
    {
        if(strcmp(rover->next->Nname, "Unused") == 0 && differnce < rover->next->length - _length)
        {
            // Nname = Node name , if the next node is "unused" && process length is less than the length of "unused"
            ret = rover; //return the node before "unused"
            differnce = rover->next->length - _length;
        }
    

        rover = rover->next; // only does this if it's not unused, moves from first node to next node
    }

    return ret;
}

void Release(char _name[], struct Node **head)
{
    struct Node *rover = NULL;
    rover = *head;
    int begone = 0;
    int begone2 = 0;

    while (rover != NULL /*|| strcmp(rover->Nname, _name) != 0*/) //your not at the end of the list and the name does not match
    {
        if(strcmp(rover->Nname, _name) == 0)
        {
            break;
        }
        
        rover = rover->next;
    }
    
    if(rover != NULL)//you found the process to remove
    { 
        strcpy(rover->Nname, "Unused");      

        struct Node *temp = NULL; //rover where we are, temp is immediately after
        struct Node *temp2 = NULL;  

        if(rover->next != NULL)
        {
            if (strcmp(rover->next->Nname, "Unused") == 0) //just the next is unused
            {
                temp = rover->next;
                rover->length += rover->next->length + 1;
                rover->next = temp->next;
                if(rover->next != NULL) 
                    rover->next->prev = rover;
                begone = 1;
            }
        }

        //just the prev is Unused
        if(rover->prev != NULL) //the prev could be NULL if rover is Head
        {
            if(strcmp(rover->prev->Nname, "Unused") == 0)
            {
                temp2 = rover->prev;
                rover->prev = temp2->prev;
                rover->start = temp2->start + 1;
                rover->length += temp2->length + 1;
                if(rover->prev != NULL)
                {
                    rover->prev->next = rover;
                }
                begone2 = 1;

                //add moves start so this is counterbalence
                temp2->next->start -= 1;
            }
        }

        if(rover->start == 0)
            *head = rover; //Frik this line of code in particular. This line caused at least 2 hours of pair programing to solve with 3 people

        if(begone2 == 1 && temp2 != NULL)
        {
            free(temp2);
        }

        if(begone == 1 && temp != NULL)
        {
            free(temp);
        }
    }
}