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
	struct password* customerPassword;
};

struct transaction{
	int custId;
	long currentBalance;
	long prevBalance;
	long deposit;
	long withdraw;
	struct transaction* next;
};

struct password{
	int custId;
	char currentPassword[20];
	char prevPassword[20];
	struct password* next;
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
	newCustomer->customerPassword = NULL;
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
				}else{
					struct transaction* newTransaction = (struct transaction*)malloc(sizeof(struct transaction));
					newTransaction->custId = customers->custId;
					newTransaction->prevBalance = customers->balance;
					newTransaction->deposit =0;
					customers->balance -= withdraw;
					newTransaction->currentBalance = customers->balance;
					newTransaction->withdraw = withdraw;
					newTransaction->next = NULL;
					if(customers->customerTransaction != NULL){
						struct transaction* oldTarnsaction = customers->customerTransaction;
						newTransaction->next = oldTarnsaction;
					}
					customers->customerTransaction = newTransaction;
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
				if(customers->customerTransaction != NULL){
						struct transaction* oldTarnsaction = customers->customerTransaction;
						newTransaction->next = oldTarnsaction;
					}
				customers->customerTransaction = newTransaction;
				break;
			case 3: ;
				int transferId;
				long transferMoney;
				struct bankCustomer* checkCustomers = (*customer);
				printf("\n\t***** Money Transfer *****\n");
				printf("\nEnter transfer customer id : ");
				scanf("%d",&transferId);
				while(checkCustomers!=NULL){
					if(checkCustomers->custId == transferId && checkCustomers->custId != customers->custId)
						break;
					checkCustomers = checkCustomers->next;
				}
				if(checkCustomers == NULL){
					printf("\n\t***** Customer not found *****\n");
				}else{
					printf("\nEnter transfer money : ");
					scanf("%ld",&transferMoney);
					if(customers->balance-transferMoney < minimum_balance){
						printf("\nMinimum balance is %d",minimum_balance);
					}else{
						struct transaction* newTransaction = (struct transaction*)malloc(sizeof(struct transaction));
						newTransaction->custId = customers->custId;
						newTransaction->prevBalance = customers->balance;
						newTransaction->deposit =0;
						customers->balance -= transferMoney;
						newTransaction->currentBalance = customers->balance;
						newTransaction->withdraw = transferMoney;
						newTransaction->next = NULL;
						if(customers->customerTransaction != NULL){
							struct transaction* oldTarnsaction = customers->customerTransaction;
							newTransaction->next = oldTarnsaction;
						}
						customers->customerTransaction = newTransaction;
						//account transfer
						struct transaction* newAccountTransaction = (struct transaction*)malloc(sizeof(struct transaction));
						newAccountTransaction->custId = checkCustomers->custId;
						newAccountTransaction->prevBalance = checkCustomers->balance;
						newAccountTransaction->deposit =transferMoney;
						checkCustomers->balance += transferMoney;
						newAccountTransaction->currentBalance = checkCustomers->balance;
						newAccountTransaction->withdraw = 0;
						newAccountTransaction->next = NULL;
						if(checkCustomers->customerTransaction != NULL){
							struct transaction* oldAccountTarnsaction = checkCustomers->customerTransaction;
							newAccountTransaction->next = oldAccountTarnsaction;
						}
						checkCustomers->customerTransaction = newAccountTransaction;
					}
				}				
				break;
			case 4: ;
				struct transaction* custTarnsaction = customers->customerTransaction;
				printf("\n\t**** Transaction History ****\n");
				printf("\nName : %s\n",customers->name);
				if(custTarnsaction == NULL){
					printf("\nRecord Not Found..!!\n");	
				}else{
					int i=1;
					printf("\n\tNo.\tC.Balance\tDeposit\tWithdraw\tP.Balance\n");
					while(custTarnsaction != NULL){
						printf("\n\t%d\t%ld\t%ld\t%ld\t%ld",i,custTarnsaction->currentBalance,custTarnsaction->deposit,custTarnsaction->withdraw,custTarnsaction->prevBalance);
						i++;
						custTarnsaction = custTarnsaction->next;
					}
				}
				int trnF =1;
				while(trnF){
					int trnChc;
					printf("\n\t**** Transaction History ****\n");
					printf("\n1.Expert Transaction History\n2.Exit\nEnter your choice : ");
					scanf("%d",&trnChc);
					switch(trnChc){
						case 1: ;
							struct transaction* custTarns = customers->customerTransaction;
							FILE* transFile = fopen("transaction.txt","w");
							fprintf(transFile,"\n\t**** Transaction History ****\n");
							fprintf(transFile,"\nName : %s\n",customers->name);
							if(custTarns == NULL){
								fprintf(transFile,"\nRecord Not Found..!!\n");	
							}else{
								int i=1;
								fprintf(transFile,"\n\tNo.\tC.Balance\tDeposit\tWithdraw\tP.Balance\n");
								while(custTarns != NULL){
									fprintf(transFile,"\n\t%d\t%ld\t%ld\t%ld\t%ld",i,custTarns->currentBalance,custTarns->deposit,custTarns->withdraw,custTarns->prevBalance);
									i++;
									custTarns = custTarns->next;
								}
							}
							fclose(transFile);	
							break;
						case 2:
							trnF = 0;
							break;
					}
				}
				break;
			case 5: ;
				char currentPswd[20];
				printf("\n\t***** Password Reset *****\n");
				printf("\nEnter current password : ");
				scanf("%s",currentPswd);
				for(i=0;currentPswd[i]!='\0';i++)
					currentPswd[i] = encrptionChar(currentPswd[i]);
				if(strcmp(customers->password,currentPswd) != 0){
					printf("\n\t***** Current password not match..!! ******\n");
				}else{
					char newPswd[20],conPswd[20];
					printf("\nEnter Password : ");
					scanf("%s",newPswd);
					printf("\nEnter Confirm Password : ");
					scanf("%s",conPswd);
					if(strcmp(newPswd,conPswd) != 0){
						printf("\n*** Password not match..!! ***\n");
					}else{
						struct password* passwordChange = (struct password*)malloc(sizeof(struct password));
						passwordChange->custId = customers->custId;
						strcpy(passwordChange->prevPassword,customers->password);
						for(i=0;newPswd[i]!='\0';i++)
							newPswd[i] = encrptionChar(newPswd[i]);
						strcpy(customers->password,newPswd);
						strcpy(passwordChange->currentPassword,customers->password);
						passwordChange->next = NULL;
						if(customers->customerPassword != NULL){
							struct password* tempPswd = customers->customerPassword;
							passwordChange->next = tempPswd;
						}
						customers->customerPassword = passwordChange;
					}
				}
				break;
			case 6: ;
				struct password* custPass = customers->customerPassword;
				printf("\n\t***** Password change history *****\n");
				if(custPass == NULL)
					printf("\nNo records found..!!\n");
				else{
					int i=1;
					printf("\nName : %s",customers->name);
					printf("\n\tNo.\tCustId\tCurrentPswd\tPrePswd\n");
					while(custPass != NULL){
						printf("\n\t%d\t%d\t%s\t%s",i,custPass->custId,custPass->currentPassword,custPass->prevPassword);
						custPass = custPass->next;
						i++;
					}
				}
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
