#include<stdio.h>
#include<stdlib.h>
#include<string.h>

#define default_balance 10000
#define minimum_balance 1000
#define encryptionkey 4

struct bankCustomer{
	int custId;
	char password[20];
	char name[50];
	long balance;
	struct bankCustomer* next;
	struct transaction* customerTransaction;
};

struct transaction{
	int custId;
	long currentBalance;
	long prevBalance;
	long deposit;
	long withdraw;
	struct transaction* next;
};

char encrptionChar(char key){
	if((key >= 'a' && key <= 'z') || (key >= 'A' && key <= 'Z')){
		if((key >= 'a' && key <= 'z'))
			return (((key-'a')+encryptionkey)%26) + 'a';
		else
			return (((key-'A')+encryptionkey)%26) + 'A';
	}else if(key >= '0' && key <= '9'){
		return (((key-'0')+encryptionkey)%10) + '0';
	}else
		return key+encryptionkey;
}
void addCustomer(struct bankCustomer** customer){
	char cPswd[20],i;
	struct bankCustomer* newCustomer = (struct bankCustomer*)malloc(sizeof(struct bankCustomer));
	newCustomer->custId = 0;
	newCustomer->balance = default_balance;
	newCustomer->next = NULL;
	newCustomer->customerTransaction = NULL;
	printf("\n***** Customer Form *****\n");
	printf("\nEnter name : ");
	scanf("%s",newCustomer->name);
	printf("\nEnter Password : ");
	scanf("%s",newCustomer->password);
	printf("\nEnter Confirm Password : ");
	scanf("%s",cPswd);
	if(strcmp(newCustomer->password,cPswd) != 0){
		printf("\n*** Password not match..!! ***\n");
		free(newCustomer);
		return;
	}
	for(i=0;cPswd[i]!='\0';i++)
		cPswd[i] = encrptionChar(cPswd[i]);
	strcpy(newCustomer->password,cPswd);
	if((*customer) == NULL){
		(*customer) = newCustomer;
		return;
	}
	struct bankCustomer* lastCustomer = (*customer);
	while(lastCustomer->next != NULL){
		newCustomer->custId++;
		lastCustomer = lastCustomer->next;
	}
	newCustomer->custId++;
	lastCustomer->next = newCustomer;
}

void viewCustomer(struct bankCustomer** customer){
	struct bankCustomer* allCustomer = (*customer);
	printf("\n\t******* Bank Customers *******\n");
	if(allCustomer == NULL){
		printf("\n****** No Record Found ******\n");
		return;
	}
	printf("\n\tCustId\tName\tBalance\tPassword\n");
	while(allCustomer != NULL){
		printf("\n\t%d\t%s\t%ld\t%s\n",allCustomer->custId,allCustomer->name,allCustomer->balance,allCustomer->password);
		allCustomer = allCustomer->next;
	}
}

void extractCustomer(struct bankCustomer** customer){
	FILE* exFile = fopen("bankcustomer.txt","w");
	if(exFile == NULL){
		printf("\n\t*** oops..!! File not found..,");
		return;
	}
	struct bankCustomer* allCustomer = (*customer);
	fprintf(exFile,"\n\t******* Bank Customers *******\n");
	if(allCustomer == NULL){
		fprintf(exFile,"\n****** No Record Found ******\n");
		return;
	}
	fprintf(exFile,"\n\tCustId\tName\tBalance\tPassword\n");
	while(allCustomer != NULL){
		fprintf(exFile,"\n\t%d\t%s\t%ld\t%s\n",allCustomer->custId,allCustomer->name,allCustomer->balance,allCustomer->password);
		allCustomer = allCustomer->next;
	}
	fclose(exFile);
}

void login(struct bankCustomer** customer){
	int custId,i;
	char pswd[20];
	struct bankCustomer* customers = (*customer);
	printf("\n\t***** Login Form *****\n");
	printf("\nEnter Customer Id : ");
	scanf("%d",&custId);
	while(customers!=NULL){
		if(customers->custId == custId)
			break;
		customers = customers->next;
	}
	if(customers == NULL){
		printf("\nCustomer id not found or wrong..!!\n");
		return;
	}
	printf("\nEnter Password : ");
	scanf("%s",pswd);
	for(i=0;pswd[i]!='\0';i++)
		pswd[i] = encrptionChar(pswd[i]);
	if(strcmp(customers->password,pswd)!=0){
		printf("\nPassword not match..!!\n");
		return;
	}
	printf("\nCurrent Balance : %ld",customers->balance);
	int f=1,chc;
	while(f){
		printf("\n1.Withdraw\n2.Deposit\n3.Money Transfer\n4.View Transfer History\n5.Password Reset\n6.View Password Change History\n7.Logout\nEnter Your Choice : ");
		scanf("%d",&chc);
		switch(chc){
			case 1: ;
				long withdraw;
				printf("\nEnter withdraw money : ");
				scanf("%ld",&withdraw);
				if(customers->balance-withdraw < minimum_balance){
					printf("\nMinimum balance is %d",minimum_balance);
					break;
				}else{
					struct transaction* newTransaction = (struct transaction*)malloc(sizeof(struct transaction));
					newTransaction->custId = customers->custId;
					newTransaction->prevBalance = customers->balance;
					newTransaction->deposit =0;
					customers->balance -= withdraw;
					newTransaction->currentBalance = customers->balance;
					newTransaction->withdraw = withdraw;
					newTransaction->next = NULL;
					if(customers->customerTransaction == NULL)
						customers->customerTransaction = newTransaction;
					else{
						struct transaction* oldTarnsaction = customers->customerTransaction;
						while(oldTarnsaction->next != NULL)
							oldTarnsaction = oldTarnsaction->next;
						oldTarnsaction->next = newTransaction;
					}
				}
				break;
			case 2: ;
				long deposit;
				printf("\nEnter deposit money : ");
				scanf("%ld",&deposit);
				struct transaction* newTransaction = (struct transaction*)malloc(sizeof(struct transaction));
				newTransaction->custId = customers->custId;
				newTransaction->prevBalance = customers->balance;
				newTransaction->deposit =deposit;
				customers->balance += deposit;
				newTransaction->currentBalance = customers->balance;
				newTransaction->withdraw = 0;
				newTransaction->next = NULL;
				if(customers->customerTransaction == NULL)
					customers->customerTransaction = newTransaction;
				else{
					struct transaction* oldTarnsaction = customers->customerTransaction;
					while(oldTarnsaction->next != NULL)
						oldTarnsaction = oldTarnsaction->next;
					oldTarnsaction->next = newTransaction;
				}
				break;
			case 3:
				break;
			case 4: ;
				struct transaction* custTarnsaction = customers->customerTransaction;
				printf("\n\t**** Transaction History ****\n");
				printf("\nName : %s\n",customers->name);
				if(custTarnsaction == NULL){
					printf("\nRecord Not Found..!!\n");	
				}else{
					int i=1;
					printf("\n\tNo.\tCurrent Balance\tDeposit\tWithdraw\tPrev Balance\n");
					while(custTarnsaction != NULL){
						printf("\n\t%d\t%ld\t%ld\t%ld\t%ld",i,custTarnsaction->currentBalance,custTarnsaction->deposit,custTarnsaction->withdraw,custTarnsaction->prevBalance);
						i++;
						custTarnsaction = custTarnsaction->next;
					}
				}
				break;
			case 5:
				break;
			case 6:
				break;
			case 7:
				f=0;
				break;
			default:
				break;
		}
	}
}

int main(){
	struct bankCustomer* customers = (struct bankCustomer*)malloc(sizeof(struct bankCustomer));
	customers = NULL;
	int chc,f=1;
	while(f){
		printf("\n\t****** Banking System ******\n");
		printf("\n1.Add Customer\n2.View Customer\n3.Export customer in text file\n4.Exit\n5.Login\nEnter Your Choice : ");
		scanf("%d",&chc);
		switch(chc){
			case 1:
				addCustomer(&customers);
				f=1;
				break;
			case 2:
				viewCustomer(&customers);
				f=1;
				break;
			case 3:
				extractCustomer(&customers);
				f=1;
				break;
			case 4:
				f=0;
				break;
			case 5:
				login(&customers);
				f=1;
				break;
			default:
				printf("\nEnter valid number..!!\n");
				f=1;
				break;
		}	
	}
	return 0;
}
