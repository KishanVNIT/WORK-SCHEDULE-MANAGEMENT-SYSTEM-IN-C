#include<stdio.h>
#include<string.h>
#define NAME_SIZE 100
#define DEP_SIZE 10
#define EMP_SIZE 1000
#define TEAM_SIZE 100
struct Employee_DB
{
    int employee_id;
    char name[NAME_SIZE];
    int age;
    char department[DEP_SIZE];
    int weekly_hours[7];
    int target;
};
struct Team_DB {
    int team_id;
    char t_name[NAME_SIZE];
    int member_id[5];
    char member_name[5][NAME_SIZE];
    int member_hours[5][7];
    int curr_total;
    int team_target;
};
struct TopEmployees {
    int top_id;
    char top_name[NAME_SIZE];
    int top_hours;
    int top_target;
};
struct TopTeams {
    int top_id;
    char top_name[NAME_SIZE];
    int top_total_hours;
    int top_rank;
};
void LoadEmployees(struct Employee_DB emp_db[], int *size) {
    FILE *fp = fopen("employees.txt", "r");
    if (fp == NULL) {
        *size = 0;
        return;
    }
    *size = 0;
    while (fscanf(fp, "%d %s %d %s", 
                  &emp_db[*size].employee_id, 
                  emp_db[*size].name, 
                  &emp_db[*size].age, 
                  emp_db[*size].department) == 4) {
        for (int j = 0; j < 7; j++) {
            fscanf(fp, "%d", &emp_db[*size].weekly_hours[j]);
        }
        fscanf(fp, "%d", &emp_db[*size].target);
        (*size)++;
    }
    fclose(fp);
}
void LoadTeams(struct Team_DB t_db[], int *size,int* team_members) {
    FILE *fp = fopen("teams.txt", "r");
    if (fp==NULL) {
        *size = 0;
        return;
    }
    *size = 0;
    while (fscanf(fp, "%d %d %s",&*(team_members+(*size)), &t_db[*size].team_id, 
                  t_db[*size].t_name) == 3) {
        for (int i = 0; i < 5 && i<*(team_members+(*size)); i++) {
            fscanf(fp,"%d",&t_db[*size].member_id[i]);
            fscanf(fp,"%s",t_db[*size].member_name[i]);
            for(int j=0;j<7;j++) {
                fscanf(fp, "%d", &t_db[*size].member_hours[i][j]);
            }
        }
        fscanf(fp, "%d", &t_db[*size].team_target);
        fscanf(fp,"%d",&t_db[*size].curr_total);
        (*size)++;
    }
    fclose(fp);
}
void SaveEmployees(struct Employee_DB emp_db[], int size) {
    FILE *fp = fopen("employees.txt", "w");
    for (int i = 0; i<size; i++) {
        fprintf(fp, "%d %s %d %s ", emp_db[i].employee_id, emp_db[i].name, emp_db[i].age, emp_db[i].department);
            for (int j = 0; j < 7; j++) {
                fprintf(fp, "%d ", emp_db[i].weekly_hours[j]);
            }
            fprintf(fp, "%d\n", emp_db[i].target);
    }
    fclose(fp);
}
void SaveTeams(struct Team_DB t_db[], int size,int* team_members) {
    FILE *fp = fopen("teams.txt", "w");
    for (int i=0; i<size; i++) {
        fprintf(fp, "%d %d %s ", *(team_members+i), t_db[i].team_id, t_db[i].t_name);
        for (int j=0; j<5 && j<*(team_members+i); j++) {
            fprintf(fp, "%d %s ", t_db[i].member_id[j], t_db[i].member_name[j]);
            for (int k=0; k<7; k++) {
                fprintf(fp, "%d ", t_db[i].member_hours[j][k]);
            }
        }
        fprintf(fp, "%d %d\n", t_db[i].team_target, t_db[i].curr_total);
    }
    fclose(fp);
}
void InitializeEmployees(struct Employee_DB emp_db[], int size) {
    for (int i=0; i<size; i++)
    {
        emp_db[i].employee_id=0;
        emp_db[i].name[0]='\0';
        emp_db[i].age=0;
        emp_db[i].department[0]='\0';
        emp_db[i].weekly_hours[0]=-1;
        emp_db[i].target=0;
    }
}
void InitializeTeams(struct Team_DB t_db[],int size) {
    for(int i=0;i<size;i++) {
        t_db[i].team_id=0;
        t_db[i].t_name[0]='\0';
        int j=0;
        while(j<5) {
            t_db[i].member_id[j]=0;
            t_db[i].member_name[j][0]='\0';
            int k=0;
            while(k<7) {
                t_db[i].member_hours[j][k]=0;
                k++;
            }
            j++;
        }
        t_db[i].curr_total=0;
        t_db[i].team_target=0;
    }
}
int CheckMemberinTeam(struct Team_DB t_db[],int id,int* t,int* team_members,int* team_no) {
    int found=0;
    *team_no=0;
    while((*team_no)<=(*t) && !found) {
        int j=0;
        while(j<*(team_members+(*team_no)) && !found) {
            if(t_db[*team_no].member_id[j]==id) found=1;
            j++;
        }
        (*team_no)++;
    }
    (*team_no)--;
    return found;
}
void UpdateMemberinTeam(struct Team_DB t_db[],int* t,int id,char n[],int h[],int* team_members) {
    int team_no=0;
    int* p=&team_no;
    if(CheckMemberinTeam(t_db,id,t,team_members,p)) {
        int i=0;
        while(t_db[team_no].member_id[i]!=id) i++;
        int k=0,pretotal=0;
        while(k<7) {
            
            pretotal=pretotal+t_db[team_no].member_hours[i][k];
            k++;
        }
        t_db[team_no].curr_total-=pretotal;
        strcpy(t_db[team_no].member_name[i],n);

        int total=0;
        k=0;
        while(k<7) {
            t_db[team_no].member_hours[i][k]=h[k];
            total=total+t_db[team_no].member_hours[i][k];
            k++;
        }
         t_db[team_no].curr_total+=total;


        

    }
}
void InsertUpdateEmployee(struct Employee_DB emp_db[],int id, char n[],int a,char d[],int h[],int t,int* e) {
    int i=0;
    while((i<*e) && (emp_db[i].employee_id<id)) {
            i++;
    }  //found place at which employee is to be inserted or updated
    if(i==EMP_SIZE) {
        printf("Record list full! Please delete atleast 1 record to add.");
    }
    else {
        if(emp_db[i].employee_id==id) {
            strcpy(emp_db[i].name,n);
            emp_db[i].age=a;
            strcpy(emp_db[i].department,d);
            for(int j=0;j<7;j++) {
                emp_db[i].weekly_hours[j]=h[j];
            }
            emp_db[i].target=t;
            
            printf("\nRecord updated successfully!\n\n");
        }
        else {
            if(*e==EMP_SIZE)
                printf("Record list full! Please delete atleast 1 record to add.");
            else {
                int j=*e;
                while(j>i) {
                    emp_db[j].employee_id=emp_db[j-1].employee_id;
                    strcpy(emp_db[j].name,emp_db[j-1].name);
                    emp_db[j].age=emp_db[j-1].age;
                    strcpy(emp_db[j].department,emp_db[j-1].department);
                    for(int k=0;k<7;k++) {
                        emp_db[j].weekly_hours[k]=emp_db[j-1].weekly_hours[k];
                    }
                    emp_db[j].target=emp_db[j-1].target;
                    j--;  
                }  //Shifted each record up 1 place
                emp_db[i].employee_id=id;
                strcpy(emp_db[i].name,n);
                emp_db[i].age=a;
                strcpy(emp_db[i].department,d);
                for(int k=0;k<7;k++) {
                    emp_db[i].weekly_hours[k]=h[k];
                }
                emp_db[i].target=t;
                printf("\nRecord added successfully!\n\n");
                *e=*e+1;
            }
        }
    }
    
}
int CheckMemberinRecord(struct Employee_DB emp_db[],int id,int* e) {
    int found=0,i=0;
    while(i<*e && !found) {
        if(emp_db[i].employee_id==id) found=1;
    i++;
    }
    return found;
}
int DeleteEmployee(struct Employee_DB emp_db[],int id,int* e) {
    int status=0;
    if(CheckMemberinRecord(emp_db,id,e)) {
        int i=0;
        while((i<*e)&&(emp_db[i].employee_id<id)) i++;
        int j=i;
        while(j<*e-1) {
            emp_db[j].employee_id=emp_db[j+1].employee_id;
            strcpy(emp_db[j].name,emp_db[j+1].name);
            emp_db[j].age=emp_db[j+1].age;
            strcpy(emp_db[j].department,emp_db[j+1].department);
            for(int k=0;k<7;k++) {
                emp_db[j].weekly_hours[k]=emp_db[j+1].weekly_hours[k];
            }
            emp_db[j].target=emp_db[j+1].target;
            j++;
        }  //Shifted each record down 1 place
        emp_db[j].employee_id=0;
        emp_db[j].name[0]='\0';
        emp_db[j].age=0;
        emp_db[j].department[0]='\0';
        emp_db[j].weekly_hours[0]=-1;
        emp_db[j].target=0;  //Initialized last record
        *e=*e-1;
        printf("\nRecord deleted successfully!\n\n");
        status=1;
    }
    else
        printf("\nRecord not found. Please enter correct employee ID!\n\n");
    return status;
}
void ShowRecords (struct Employee_DB emp_db[],int* e) {
    for(int i=0;i<*e;i++) {
        printf("\n%d.",i+1);
        printf("ID: %d\n",emp_db[i].employee_id);
        printf("  Name: %s\n",emp_db[i].name);
        printf("  Age: %d\n",emp_db[i].age);
        printf("  Department: %s\n",emp_db[i].department);
        printf("  Working hours from Monday to Sunday: ");
        for(int j=0;j<7;j++) {
            printf("%d ",emp_db[i].weekly_hours[j]);
        }
        printf("\n  Target hours: %d\n\n",emp_db[i].target);
    }
    printf("\n\n");
}
int CheckTeaminRecord(struct Team_DB t_db[],int id,int* t) {
    int found=0,i=0;
    while(i<*t && !found) {
        if(t_db[i].team_id==id) found=1;
    i++;
    }
    return found;
}
void CreateUpdateTeam (struct Team_DB t_db[],int id,char n[],int mid[],int tt,int* t,struct Employee_DB emp_db[],int* e,int* team_members) {
    int i=0,member_pos=0,total=0,team_no=0;
    int* p=&team_no;
    while((i<*t)&&(t_db[i].team_id<id)) {
        i++;
    }  //found place at which team is to be inserted or updated
    if(i==TEAM_SIZE) {
        printf("\nTeam List full! Cannot add more teams.\n");
    }
    else {
        if(t_db[i].team_id==id) {
            int j=0;
            while(j<*(team_members+i)) {
                t_db[i].member_id[j]=0;
                t_db[i].member_name[j][0]='\0';
                int k=0;
                while(k<7) {
                    t_db[i].member_hours[j][k]=0;
                    k++;
                }
                j++;
            }
            *(team_members+i)=0;
            strcpy(t_db[i].t_name,n);
            for(int j=0;j<5&&mid[j]!=0;j++) {
                if(CheckMemberinRecord(emp_db,mid[j],e)==1)
                    if(CheckMemberinTeam(t_db,mid[j],t,team_members,p)==0) {
                        *(team_members+i)=*(team_members+i)+1;
                        int k=0;
                        while(emp_db[k].employee_id<mid[j]) k++;  //found the location of employee with id mid[j] in employee recs
                        t_db[i].member_id[member_pos]=emp_db[k].employee_id;
                        strcpy(t_db[i].member_name[member_pos],emp_db[k].name);
                        for(int m=0;m<7;m++) {
                            t_db[i].member_hours[member_pos][m]=emp_db[k].weekly_hours[m];
                            total=total+emp_db[k].weekly_hours[m];
                        }
                        member_pos++;
                    }
                    else printf("Member with ID %d already exists in another team\n",mid[j]);
                else printf("Member with ID %d not in records!\n",mid[j]);
            }
            t_db[i].team_target=tt;
            t_db[i].curr_total=total;
            printf("\nTeam updated successfully!\n\n");
        }
        else {
            if(*t==TEAM_SIZE) printf("\nTeam List full! Cannot add more teams.\n");
            else {
                int j=*t;
                while(j>i) {
                    t_db[j].team_id=t_db[j-1].team_id;
                    strcpy(t_db[j].t_name,t_db[j-1].t_name);
                    for(int k=0;k<5;k++) {
                        t_db[j].member_id[k]=t_db[j-1].member_id[k];
                        strcpy(t_db[j].member_name[k],t_db[j-1].member_name[k]);
                        for(int m=0;m<7;m++) {
                            t_db[j].member_hours[k][m]=t_db[j-1].member_hours[k][m];
                        }
                    }
                    t_db[j].team_target=t_db[j-1].team_target;
                    t_db[j].curr_total=t_db[j-1].curr_total;
                    // t_db[j].team_rank=t_db[j-1].team_rank;
                    *(team_members+j)=*(team_members+j-1); 
                    j--; 
                }  //Shifted each record up 1 place
                *(team_members+i)=0;
                j=0;
                while(j<5) {
                    t_db[i].member_id[j]=0;
                    t_db[i].member_name[j][0]='\0';
                    int k=0;
                    while(k<7) {
                        t_db[i].member_hours[j][k]=0;
                        k++;
                    }
                    j++;
                }
                member_pos=0;
                t_db[i].team_id=id;
                strcpy(t_db[i].t_name,n);
                for(int j=0;j<5 && mid[j]!=0;j++) {
                    if(CheckMemberinRecord(emp_db,mid[j],e)==1)
                        if(CheckMemberinTeam(t_db,mid[j],t,team_members,p)==0) {
                            *(team_members+i)=*(team_members+i)+1;
                            int k=0;
                            while(emp_db[k].employee_id<mid[j]) k++;
                            t_db[i].member_id[member_pos]=emp_db[k].employee_id;
                            strcpy(t_db[i].member_name[member_pos],emp_db[k].name);
                            for(int m=0;m<7;m++) {
                                t_db[i].member_hours[member_pos][m]=emp_db[k].weekly_hours[m];
                                total=total+emp_db[k].weekly_hours[m];
                            }
                            member_pos++;
                        }
                        else printf("Member with ID %d already exists in another team\n",mid[j]);
                    else printf("Member with ID %d not in records!\n",mid[j]);
                }
                t_db[i].team_target=tt;
                t_db[i].curr_total=total;
                printf("\nTeam added successfully!\n\n");
                *t=*t+1;
            }
        }
    }
}
void RemoveMemberfromTeam(struct Team_DB t_db[],int id,int* t, int* team_members) {
    int team_no=0;
    int* p=&team_no;
    if(CheckMemberinTeam(t_db,id,t,team_members,p)) {
        int i=0;
        while(t_db[team_no].member_id[i]<id) i++;
        int total=0;
        for(int k=0;k<7;k++) total=total+t_db[team_no].member_hours[i][k];
        t_db[team_no].curr_total=t_db[team_no].curr_total-total;
        int j=i;
        while(j<(*(team_members+team_no)-1)) {
            t_db[team_no].member_id[j]=t_db[team_no].member_id[j+1];
            strcpy(t_db[team_no].member_name[j],t_db[team_no].member_name[j+1]);
            int k=0;
            for(int k=0;k<7;k++)
                t_db[team_no].member_hours[j][k]=t_db[team_no].member_hours[j+1][k];
            j++;
        }
        t_db[team_no].member_id[j]=0;
        t_db[team_no].member_name[j][0]='\0';
        for(int k=0;k<7;k++)
            t_db[team_no].member_hours[j][k]=0;
        (*(team_members+(team_no)))--;
        printf("Member also deleted from the team with ID %d\n\n",t_db[team_no].team_id);
    }
}
void DeleteTeam(struct Team_DB t_db[],int id,int* t,int* team_members) {
    if(CheckTeaminRecord(t_db,id,t)) {
        int i=0;
        while((i<*t)&&(t_db[i].team_id<id)) {
                i++;
        }  //now ith position contains either req id or greater than that
        int j=i;
        while(j<*t-1) {
            t_db[j].team_id=t_db[j+1].team_id;
            strcpy(t_db[j].t_name,t_db[j+1].t_name);
            for(int k=0;k<5;k++) {
                t_db[j].member_id[k]=t_db[j+1].member_id[k];
                strcpy(t_db[j].member_name[k],t_db[j+1].member_name[k]);
                for(int l=0;l<7;l++)
                    t_db[j].member_hours[k][l]=t_db[j+1].member_hours[k][l];
            }
            t_db[j].curr_total=t_db[j+1].curr_total;
            t_db[j].team_target=t_db[j+1].team_target;
            // t_db[j].team_rank=t_db[j+1].team_rank;
            *(team_members+j)=*(team_members+j+1);
            j++;
        }  //Shifted each record down 1 place
        *(team_members+j)=0;
        t_db[j].team_id=0;
        t_db[j].t_name[0]='\0';
        int k=0;
        while(k<5) {
            t_db[j].member_id[k]=0;
            t_db[j].member_name[k][0]='\0';
            for(int l=0;l<7;l++)
                t_db[j].member_hours[k][l]=0;
            k++;
        }
        t_db[j].curr_total=0;
        t_db[j].team_target=0;
        // t_db[j].team_rank=0;  //Initialized last record
        *t=*t-1;
        printf("\nTeam record deleted successfully!\n\n");
    }
    else
        printf("\nRecord not found. Please enter correct team ID!\n\n");
}
void GenerateRank(struct Team_DB t_db[],struct TopTeams top_team[],int *t) {
     
    for(int i=0;i<*t;i++) {
        top_team[i].top_total_hours=0;
    }  
    
    int i=0;
    while(i<*t) {
        int k=0;
        while(k<*t && top_team[k].top_total_hours>t_db[i].curr_total && top_team[k].top_total_hours!=0) {
            k++;
        }  //found the appropriate position of the team on basis of its total
        if(top_team[k].top_total_hours==0) {
            top_team[k].top_total_hours=t_db[i].curr_total;
            top_team[k].top_id=t_db[i].team_id;
            strcpy(top_team[k].top_name,t_db[i].t_name);  
        }  //if the appropriate is empty just put the team at that position
        else {
            int l=*t-1;
            while(l>k && top_team[l].top_total_hours==0) {
                l--;
            }
            l=l+1;  //l is the first location where tophours=0
            while(l>k) {
                top_team[l].top_total_hours=top_team[l-1].top_total_hours;
                top_team[l].top_id=top_team[l-1].top_id;
                strcpy(top_team[l].top_name,top_team[l-1].top_name);
                l--;
            }  //shifting each team up 1 place
            top_team[k].top_total_hours=t_db[i].curr_total;
            top_team[k].top_id=t_db[i].team_id;
            strcpy(top_team[k].top_name,t_db[i].t_name);
            
        }  
        
        i++;
    }

   
}
void DisplayTeamInfo (struct Team_DB t_db[],int* t,struct TopTeams top_team[],int* team_members) {
    GenerateRank(t_db,top_team,t);

    for(int i=0;i<*t;i++) {
        printf("\n%d.",i+1);
        printf("Team ID: %d\n",t_db[i].team_id);
        printf("  Team Name: %s\n",t_db[i].t_name);
        printf("  \nMembers: ");
        for(int j=0;j<*(team_members+i);j++) {
            printf("\n\n\t%d)",j+1);
            printf("ID:%d\n",t_db[i].member_id[j]);
            printf("\t  Name:%s\n",t_db[i].member_name[j]);
            printf("\t  Working hours from Monday to Sunday: ");
            for(int k=0;k<7;k++) {
                printf("%d ",t_db[i].member_hours[j][k]);
            } 
        }
        int j=0;
        while(top_team[j].top_id!=t_db[i].team_id){
            j++;
        }
        printf("\n\n  Team Rank : %d\n",j+1);
        printf("  Team target hours: %d\n",t_db[i].team_target);
        printf("  Current total: %d\n\n\n",t_db[i].curr_total);
    }
    printf("\n\n");
}
void Top5Employees(struct Employee_DB emp_db[],struct TopEmployees top_emp[],int* e) {
    for(int i=0;i<5;i++) {
        top_emp[i].top_hours=0;
    }  //Initialization
    int i=0;
    while(i<*e) {
        int j=0,total=0;
        while(j<7) {
            total=total+emp_db[i].weekly_hours[j];
            j++;
        }
        if(total>=emp_db[i].target) {
            int k=0;
            while(k<5 && top_emp[k].top_hours>total && top_emp[k].top_hours!=0) {
                k++;
            }  //found the appropriate position of the employee on basis of its total
            if(k<5) {
                if(top_emp[k].top_hours==0) {
                    top_emp[k].top_id=emp_db[i].employee_id;
                    strcpy(top_emp[k].top_name,emp_db[i].name);
                    top_emp[k].top_hours=total;
                    top_emp[k].top_target=emp_db[i].target;
                }  //if the appropriate is empty just put the employee at that position
                else {
                    int l=4;
                    if(top_emp[4].top_hours==0) {
                        while(l>k && top_emp[l].top_hours==0) {
                            l--;
                        }
                        l=l+1;
                    }  //l is the first location where tophours=0 or if all 5 positions are filled then l=4
                    while(l>k) {
                        top_emp[l].top_id=top_emp[l-1].top_id;
                        strcpy(top_emp[l].top_name,top_emp[l-1].top_name);
                        top_emp[l].top_hours=top_emp[l-1].top_hours;
                        top_emp[l].top_target=top_emp[l-1].top_target;
                        l--;
                    }  //shifting each employee up 1 place
                    top_emp[k].top_id=emp_db[i].employee_id;
                    strcpy(top_emp[k].top_name,emp_db[i].name);
                    top_emp[k].top_hours=total;
                    top_emp[k].top_target=emp_db[i].target;  //placed the current employee at its position
                }
            }
        }
        i++;
    }
    
}
void CheckTeamAchievement(struct Team_DB t_db[],int *t,int id){
    int i=0;
    while(i<*t && t_db[i].team_id<id) i++;
    if(id!=t_db[i].team_id){
        printf("\nTeam with ID %d Does Not exist!\n\n",id);
    }
    else{
        if(t_db[i].curr_total>=t_db[i].team_target) 
            printf("\nTeam with ID %d has achieved its Weekly Team Target.\n\n",id);
        else
            printf("\nTeam with ID %d has NOT achieved its Weekly Team Target.\n\n",id);
    }
}
void MergeTeams(struct Team_DB t_db[],int *t,int teamid1,int teamid2,int newid,char newn[],int newtt,struct Employee_DB emp_db[],int* e,int* team_members){
    int i=0;
    while(i<*t && t_db[i].team_id<teamid1) i++;
    int j=0;
    while(j<*t && t_db[j].team_id<teamid2) j++;
    int newmembers[5];
    for(int x=0;x<5;x++) {
        newmembers[x]=0;
    }
    int count=0;
    int x=0,y=0;
    while(count<5 && x<*(team_members+i) && y<*(team_members+j)) {
        if(t_db[i].member_id[x]<t_db[j].member_id[y]) {
            newmembers[count]=t_db[i].member_id[x];
            x++;
            count++;
        }
        else {
            newmembers[count]=t_db[j].member_id[y];
            y++;
            count++;  
        }
    }
        while(x<*(team_members+i) && count<5) {
            newmembers[count]=t_db[i].member_id[x];
            x++;
            count++;
        }
    
        while(y<*(team_members+j) && count<5) {
            newmembers[count]=t_db[j].member_id[y];
            y++;
            count++;
        }
    
    DeleteTeam(t_db,teamid1,t,team_members);
    DeleteTeam(t_db,teamid2,t,team_members);
    CreateUpdateTeam(t_db,newid,newn,newmembers,newtt,t,emp_db,e,team_members);
}
void RewardEmployee(struct Employee_DB emp_db[],int *e,int ID,struct TopEmployees top_emp[] ){
    if(CheckMemberinRecord(emp_db,ID,e)){
    Top5Employees(emp_db,top_emp,e);
    if(top_emp[0].top_id==ID){
        printf("The Reward point Earned by Employee with ID %d is 100 points.\n ",ID);
    }
    else if(top_emp[1].top_id==ID){
        printf("The Reward point Earned by Employee with ID %d is 75 points.\n ",ID);
    }
    else if(top_emp[2].top_id==ID){
        printf("The Reward point Earned by Employee with ID %d is 50 points.\n ",ID);
    }
    else{
        int i=0;
        while((i<*e)&&(emp_db[i].employee_id<ID)) i++;
        int total=0;
        int j=0;
        while(j<7) {
            total=total+emp_db[i].weekly_hours[j];
            j++;
        }
        if(total>emp_db[i].target){
            printf("The Reward point Earned by Employee with ID %d is 20 points.\n ",ID);
        }
        else if(total==emp_db[i].target){
            printf("The Reward point Earned by Employee with ID %d is 10 points.\n ",ID);
        }
        else{
            printf("The Reward point Earned by Employee with ID %d is 0 points.\n ",ID);
        }
    }
}
else{
    printf("Member is NOT in record.\n");
}
}
int main() {
    struct Employee_DB emp[EMP_SIZE];
    struct Team_DB team[TEAM_SIZE];
    struct TopEmployees toppers[5];
    struct TopTeams top_teams[TEAM_SIZE];
    InitializeEmployees(emp,EMP_SIZE);
    InitializeTeams(team,TEAM_SIZE);
    int input;
     int flag=1;
    int num_recs;
    int e_id, e_age, e_target, e_hours[7];
    char emp_name[NAME_SIZE], emp_dep[DEP_SIZE];
    
    int curr_employee_size=0, curr_team_size=0, curr_member_size[TEAM_SIZE];
    for(int i=0;i<TEAM_SIZE;i++) curr_member_size[i]=0;

    int t_id, t_target, t_mid[5];
    for(int i=0;i<5;i++) t_mid[i]=0;
    char t_name[NAME_SIZE];
    int* curr_emp=&curr_employee_size;
    int* curr_teams=&curr_team_size;
    int* curr_members=curr_member_size;
    LoadEmployees(emp,curr_emp);
    LoadTeams(team,curr_teams,curr_members);
    printf("\n__________WORK_SCHEDULE_MANAGEMENT_SYSTEM_____________\n");
    
    while(flag) {
        printf("\n1.Add or Update Employee\n2.Delete an Employee\n3.Display Employee Info\n4.Create or Update Team\n5.Delete a Team\n6.Display team info\n7.Top 5 Employees of the week\n8.Check Team Achievement\n9.Generate Team Rank\n10.Merge Teams \n11.Check Employee Reward\n12.Exit\n\nEnter input: ");
        scanf("%d",&input);
        if(input==1) {
            int choice;
            printf("\n1.Add a new employee\n2.Update an existing employee\n\nEnter your choice: ");
            scanf("%d",&choice);
            if(choice==1) {
                printf("\nHow many employees to add? ");
                scanf("%d",&num_recs);
                for(int i=0;i<num_recs;i++) {
                    printf("\nEnter employee ID: ");
                    scanf("%d",&e_id);
                    if(CheckMemberinRecord(emp,e_id,curr_emp)) printf("\nEmployee with ID %d already exists!\n\n",e_id);
                    else {
                        if(e_id>0) {
                            printf("Enter the name of employee: ");
                            scanf("%s",emp_name);
                            printf("Enter the age of employee: ");
                            scanf("%d",&e_age);
                            printf("Enter the department of employee: ");
                            scanf("%s",emp_dep);
                            printf("Enter the weekly working hours of employee(from Monday to Sunday): ");
                            for(int j=0;j<7;j++) {
                                scanf("%d",&e_hours[j]);
                            }
                            printf("Enter target hours of employee(Max 50 hrs): ");
                            scanf("%d",&e_target);
                            InsertUpdateEmployee(emp,e_id,emp_name,e_age,emp_dep,e_hours,e_target,curr_emp);
                        }
                        else printf("\nEmployee ID cannot be less than 1. Please enter valid ID.\n\n");
                    }
                }
            }
            else if(choice==2) {
                printf("\nHow many employees to update? ");
                scanf("%d",&num_recs);
                for(int i=0;i<num_recs;i++) {
                    printf("\nEnter employee ID: ");
                    scanf("%d",&e_id);
                    if(!CheckMemberinRecord(emp,e_id,curr_emp)) printf("\nEmployee with ID %d does not exist!\n\n",e_id);
                    else {
                        printf("Enter the name of employee: ");
                        scanf("%s",emp_name);
                        printf("Enter the age of employee: ");
                        scanf("%d",&e_age);
                        printf("Enter the department of employee: ");
                        scanf("%s",emp_dep);
                        printf("Enter the weekly working hours of employee(from Monday to Sunday): ");
                        for(int j=0;j<7;j++) {
                            scanf("%d",&e_hours[j]);
                        }
                        printf("Enter target hours of employee(Max 50 hrs): ");
                        scanf("%d",&e_target);
                        InsertUpdateEmployee(emp,e_id,emp_name,e_age,emp_dep,e_hours,e_target,curr_emp);
                        UpdateMemberinTeam(team,curr_teams,e_id,emp_name,e_hours,curr_members);
                    }
                }
            }
            else printf("\nPlease enter valid choice!\n\n");
        }
        else if(input==2) {
            printf("\nHow many records to delete? ");
            scanf("%d",&num_recs);
            for(int i=0;i<num_recs;i++) {
                printf("\nEnter the employee ID: ");
                scanf("%d",&e_id);
                if(DeleteEmployee(emp,e_id,curr_emp)) {
                    RemoveMemberfromTeam(team,e_id,curr_teams,curr_members);
                };
            }
        }
        else if(input==3) {
            ShowRecords(emp,curr_emp);
        }
        else if(input==4) {
            int choice;
            printf("\n1.Add a new team\n2.Update an existing team\n\nEnter your choice: ");
            scanf("%d",&choice);
            if(choice==1) {
                printf("\nHow many teams to add? ");
                scanf("%d",&num_recs);
                for(int i=0;i<num_recs;i++) {
                    printf("\nEnter team ID: ");
                    scanf("%d",&t_id);
                    if(CheckTeaminRecord(team,t_id,curr_teams)) printf("\nTeam with ID %d already exists!\n\n",t_id);
                    else {
                        if(t_id>0) {
                            printf("Enter the name of team: ");
                            scanf("%s",t_name);
                            printf("Enter the IDs of team members(maximum 5) or enter 0 if done: ");
                            int done=0;
                            for(int j=0;j<5 && !done;j++) {
                                scanf("%d",&t_mid[j]);
                                if(t_mid[j]==0) done=1;
                            }
                            printf("Enter target hours of Team: ");
                            scanf("%d",&t_target);
                            for(int k=0;k<4 && t_mid[k]!=0;k++) {
                                int min=k;
                                for(int j=k+1;j<5 && t_mid[j]!=0;j++) {
                                    if(t_mid[j]<t_mid[min]) {
                                        min=j;
                                    } // selection sort to sort member IDs.
                                }
                                int temp=t_mid[min];
                                t_mid[min]=t_mid[k];
                                t_mid[k]=temp;
                            }
                            CreateUpdateTeam(team,t_id,t_name,t_mid,t_target,curr_teams,emp,curr_emp,curr_members);
                        }
                        else printf("\nTeam ID cannot be less than 1. Please enter valid ID.\n\n");
                    }
                }
            }
            else if(choice==2) {
                printf("\nHow many teams to update? ");
                scanf("%d",&num_recs);
                for(int i=0;i<num_recs;i++) {
                    printf("\nEnter team ID: ");
                    scanf("%d",&t_id);
                    if(!CheckTeaminRecord(team,t_id,curr_teams)) printf("\nTeam with ID %d does not exist!\n\n",t_id);
                    else {
                        printf("Enter the name of team: ");
                        scanf("%s",t_name);
                        printf("Enter the IDs of team members(maximum 5): ");
                        for(int j=0;j<5;j++) {
                            scanf("%d",&t_mid[j]);
                        }
                        printf("Enter target hours of employee: ");
                        scanf("%d",&t_target);
                        for(int k=0;k<4&&t_mid[k]!=0;k++) {
                            int min=k;
                            for(int j=k+1;j<5&&t_mid[k]!=0;j++) {
                                if(t_mid[j]<t_mid[min]) {
                                    min=j;
                                }
                            }
                            int temp=t_mid[min];
                            t_mid[min]=t_mid[k];
                            t_mid[k]=temp;
                        }
                        CreateUpdateTeam(team,t_id,t_name,t_mid,t_target,curr_teams,emp,curr_emp,curr_members);
                    }
                }    
            }
            else printf("\nPlease enter valid choice!\n\n");
        }
        else if(input==5) {
            printf("How many teams to delete? ");
            scanf("%d",&num_recs);
            for(int i=0;i<num_recs;i++) {
                printf("Enter the team ID: ");
                scanf("%d",&t_id);
                DeleteTeam(team,t_id,curr_teams,curr_members);
            }
        }
        else if(input==6) {
            DisplayTeamInfo(team,curr_teams,top_teams,curr_members);
        }
        else if(input==7) { 
            Top5Employees(emp,toppers,curr_emp);
            printf("\n\nThe Top 5 Employees of the week are :\n");
    for(int i=0;i<5 && toppers[i].top_hours!=0;i++) {
        printf("\nRank %d:",i+1);
        printf(" ID: %d\n",toppers[i].top_id);
        printf("\tName: %s\n",toppers[i].top_name);
        printf("\tTotal working hours of the week: %d\n",toppers[i].top_hours);
        printf("\tWeekly target hours: %d\n\n",toppers[i].top_target);
        if(toppers[i].top_hours-toppers[i].top_target>(toppers[i].top_target/2)) printf("\tYou have exceeded your target by more than 50%. You can have a time off !\n\n");
    }

        }
        else if(input==8) {
            printf("Enter the Team ID: ");
            scanf("%d",&t_id);
            CheckTeamAchievement(team,curr_teams,t_id);
        }
        else if(input==9) {
            GenerateRank(team,top_teams,curr_teams);
             
    printf("\n\nRanking of The Teams are :\n");
    for(int i=0;i<*curr_teams;i++) {
        printf("\nRank %d:",i+1);
        printf(" Team ID: %d\n",top_teams[i].top_id);
        printf("\tTeam Name: %s\n",top_teams[i].top_name);
        printf("\tTotal working hours of the Team: %d\n",top_teams[i].top_total_hours);
    }
    printf("\n\n");
        }
        else if(input==10 ) {

            int id1,id2,newtarget,newid;
            char newname[NAME_SIZE];
            printf("Enter team ID 1: ");
            scanf("%d",&id1);
            if(CheckTeaminRecord(team,id1,curr_teams)) {
                printf("Enter team ID 2: ");
                scanf("%d",&id2);
                if(CheckTeaminRecord(team,id2,curr_teams) && id1!=id2) {
                    printf("Enter new team ID: ");
                    scanf("%d",&newid);
                    if(newid>0) {
                        printf("Enter new team name: ");
                        scanf("%s",newname);
                        printf("Enter New target hours of Team : ");
                        scanf("%d",&newtarget);
                        MergeTeams(team,curr_teams,id1,id2,newid,newname,newtarget,emp,curr_emp,curr_members);
                    }
                    else printf("\nTeam ID cannot be less than 1. Please enter valid ID.\n\n");
                }
                else if(id1!=id2) printf("\nTeam with ID %d does not exist!\n\n",id2);
                else printf("\nTeams with same ID cannot be merged! Please enter different Team IDs.\n\n");
            }
            else printf("\nTeam with ID %d does not exist!\n\n",id1);
        }
        else if(input==11){
            printf("Enter the Employee ID");
            scanf("%d",&e_id);
            RewardEmployee(emp,curr_emp,e_id,toppers);
        }
        else if(input==12) {
            SaveEmployees(emp,*curr_emp);
            SaveTeams(team,*curr_teams,curr_members);
            flag=0;
        }
    }
    return 0;
}