#include<stdio.h>
#include<stdlib.h>
#include<string.h>

// Bus Details
struct busBook{
	int busId;
	char name[50];
	int code;
	int seats;
	char from[20];
	char to[20];
	float depTime;
	float travelHours;
	int bookingCount;
	struct busBook* next;
	struct bookingDetails* booking;
};

// Booking Details
struct bookingDetails{
	int bookId;
	char from[20];
	char to[20];
	int seats;
	struct bookingDetails* next;
	struct customerDetails* customer;
};

// Customer Details
struct customerDetails{
	int bookId;
	char name[50];
	char gender[1];
	int seatId;
	struct customerDetails* next;
};

//function Declaration

void viewSchedule(struct busBook** transport);
void ticketBooking(struct busBook** transport);

// Add Transport

void addTransport(struct busBook** transport){
	int busId,seats,code;
	float depTime,travelHours;
	char from[20],to[20],name[50];
	struct busBook* lastBus;
	lastBus = (*transport);
	struct busBook* newBus = (struct busBook*)malloc(sizeof(struct busBook));
	printf("\n\t Transport Fourm \n");
	printf("\n\tEnter Bus Id : ");
	scanf("%d",&busId);
	printf("\n\tEnter Bus Name : ");
	scanf("%s",name);
	printf("\n\tEnter Bus Code : ");
	scanf("%d",&code);
	printf("\n\tEnter Seats : ");
	scanf("%d",&seats);
	printf("\n\tEnter Depature Place : ");
	scanf("%s",from);
	printf("\n\tEnter Arrival Place : ");
	scanf("%s",to);
	printf("\n\tEnter Depature Time : ");
	scanf("%f",&depTime);
	printf("\n\tEnter Travel Hours : ");
	scanf("%f",&travelHours);
	newBus->busId = busId;
	newBus->code = code;
	newBus->seats = seats;
	newBus->depTime = depTime;
	newBus->travelHours = travelHours;
	strcpy(newBus->name,name);
	strcpy(newBus->from,from);
	strcpy(newBus->to,to);
	newBus->booking=0;
	newBus->next = NULL;
	newBus->booking = NULL;
	if((*transport) == NULL){
		(*transport) = newBus;	
		return;
	}
	else{
		while(lastBus->next!=NULL)
			lastBus = lastBus->next;
		lastBus->next = newBus;
		return;
	}	
}

// Schedule 

void viewSchedule(struct busBook** transport){
	int i,chc,f=1,busId;
	struct busBook* trans;
	trans = (*transport);
	printf("\n\t********** Transport Schedule **********\n");
	if(trans == NULL){
		printf("\n\t\t NO Records Found\n");
		return;	
	}
	printf("\n\tNo.\tBus Id\tBus Name\tAv.Seats\tFrom\tTo\tDep.Time\tTrv.Hrs\tBus Code\n");
	for(i=1;trans!=NULL;i++){
		printf("\n\t%d\t%d\t%s\t%d\t%s\t%s\t%.2f\t%.2f\t%d",i,trans->busId,trans->name,trans->seats,trans->from,trans->to,trans->depTime,trans->travelHours,trans->code);
		trans = trans->next;
	}
	while(f){
		printf("\n\n\t1.View Booking History\n\t2.Exit\n");
		printf("Enter Your Choice : ");
		scanf("%d",&chc);
		switch(chc){
			case 1:
				printf("\n\tEnter Bus Id :");
				scanf("%d",&busId);
				trans = (*transport);
				while(trans!=NULL){
					if(trans->busId == busId){
						struct bookingDetails* tBook =  trans->booking;
						if(tBook == NULL){
							printf("\n\tNo Booking Details Found\n");
						}else{
							printf("\n\tBookId\tFrom\tTo\tSeats\n");
							while(tBook != NULL){
								printf("\n\t%d\t%s\t%s\t%d",tBook->bookId,tBook->from,tBook->to,tBook->seats);	
								tBook = tBook->next;							
							}
						}
						break;	
					}
					trans = trans->next;
				}
				f=1;
				break;
			case 2:
				f=0;
				break;
		}
	}
}

// Ticket Booking

void ticketBooking(struct busBook** transport){
	int bookId,seats,busCount=0,busId,tempSeats,i;
	char from[20],to[20];
	struct busBook* busDetails = (*transport);
	if(busDetails == NULL){
		printf("\n\t\t No Records Found\n");
		return;	
	}
	printf("\n\tEnter Depature Place : ");
	scanf("%s",from);
	printf("\n\tEnter Arrival Place : ");
	scanf("%s",to);
	printf("\n\tEnter Seats : ");
	scanf("%d",&seats);
	printf("\n\tBus Id\tBus Name\tAv.Seats\tFrom\tTo\tDep.Time\tTrv.Hrs\n");
	while(busDetails != NULL){
		if(strcmp(busDetails->from,from)==0 && strcmp(busDetails->to,to)==0 && busDetails->seats>= seats){
			printf("\n\t%d\t%s\t%d\t%s\t%s\t%.2f\t%.2f",busDetails->busId,busDetails->name,busDetails->seats,busDetails->from,busDetails->to,busDetails->depTime,busDetails->travelHours);
			busCount++;
		}
		busDetails = busDetails->next;
	}
	if(busCount==0)
		printf("\n\t\t No Records Found\n");
	else{
		busDetails = (*transport);
		printf("\n\n\tEnter Bus Id : ");
		scanf("%d",&busId);
		printf("\n\tConfirm Seats : ");
		scanf("%d",&seats);
		while(busDetails!=NULL){
			if(busDetails->busId == busId){
				busDetails->seats -= seats;
				struct bookingDetails* newBooking = (struct bookingDetails*)malloc(sizeof(struct bookingDetails));
				newBooking->bookId =busDetails->bookingCount;
				strcpy(newBooking->from,from);
				strcpy(newBooking->to,to);
				newBooking->seats = seats;
				newBooking->next = NULL;
				newBooking->customer = NULL;
				tempSeats = seats;
				for(i=1;i<=tempSeats;i++){
					struct customerDetails* customers = (struct customerDetails*)malloc(sizeof(struct customerDetails));
					customers->bookId = newBooking->bookId;
					printf("\n\tEnter %d Customer Name : ",i);
					scanf("%s",customers->name);
					printf("\n\tEnter Gender (Male - M / Female - F / None - N) : ");
					scanf("%s",customers->gender);
					customers->seatId = 1;
					customers->next = NULL;
					if(newBooking->customer == NULL){
						newBooking->customer = customers;
					}else{
						struct customerDetails* lastCustomer = newBooking->customer;
						while(lastCustomer->next != NULL){
							lastCustomer = lastCustomer->next;
						}
						lastCustomer->next = customers;
					}
				}
				if(busDetails->booking ==NULL){
					busDetails->booking = newBooking;
				}else{
					struct bookingDetails* lastBooking = busDetails->booking;
					while(lastBooking->next != NULL)
						lastBooking = lastBooking->next;
					lastBooking->next = newBooking;
				}
				busDetails->bookingCount++;
				printf("\n\t****** Ticket Booked *****\n");
				printf("\n\tBus Name : %s\n\tDepature Place : %s (%.2f)\n\tArival Place : %s\n\tSelected Seats : %d\n",busDetails->name,busDetails->from,busDetails->depTime,busDetails->to,seats);
				printf("\n\n\t **** Customers ****\n\n");
				struct customerDetails* bookingCustomers = newBooking->customer;
				while(bookingCustomers!=NULL){
					printf("\n\tBookId : %d\n\tName : %s\n\tGender : %s\n\tSeatId : %d\n",bookingCustomers->bookId,bookingCustomers->name,bookingCustomers->gender,bookingCustomers->seatId);
					bookingCustomers = bookingCustomers->next;
				}
				break;
			}
			busDetails = busDetails->next;
		}
	}
}

// Main Function
int main(){
	int chc,f=1,transChc,transF=1;
	struct busBook* transport = (struct busBook*)malloc(sizeof(struct busBook));
	transport = NULL;
	while(f){
		printf("\n\t************ Transport ************\n");
		printf("\n\t1.Transport\n\t2.Schedule\n\t3.Exit\n");
		printf("\n\tEnter Your Choice : ");
		scanf("%d",&chc);
		switch(chc){
			case 1:
				transF=1;
				while(transF){
					printf("\n\t1.Add Bus Transport\n\t2.Ticket Booking\n\t3.Exit\n");
					printf("\n\tEnter Your Choice : ");
					scanf("%d",&transChc);
					switch(transChc){
						case 1:
							addTransport(&transport);
							transF = 1;
							break;
						case 2:
							ticketBooking(&transport);
							transF = 1;
							break;
						case 3:
							transF = 0;
							break;
					}
				}
				f=1;
				break;
			case 2:
				viewSchedule(&transport);
				f=1;
				break;
			case 3:
				f=0;
				break;
			default:
				printf("\n\tEnter Valid number..!!\n");
				f=1;
				break;
		}
	}
	return 0;
}
