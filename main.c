#include<stdio.h>
#include<stdlib.h>
#include<string.h>

const int MAX_TEAMS = 10;
const int MAX_PLAYERS = 20;
const char fileName[] = "football_database.txt";
const char statsFile[] = "stats.txt";

struct Player {
    char playerName[50];
    int playerNumber;
    char playerPosition[10];
    int goalsScored;
    int goalsConceded;
    int minutesPlayed;
};


///create a structure for Team
struct Team {
    char teamName[50];
    struct Player members[MAX_PLAYERS];
    int memberCount;
    int totalGoals;
    int totalGoalsConceded;
};


/// implement function loadTeamsFromFile
void loadTeamsFromFile(struct Team teams[], int *teamCount){
    FILE *file = fopen(fileName,"r");;

    if(file == NULL){
        printf("File cannot be opened.\n");
        return;
    }

    while(fscanf(file,"%s %d",teams[*teamCount].teamName,&teams[*teamCount].memberCount) == 2){
        teams[*teamCount].totalGoals = 0;
        teams[*teamCount].totalGoalsConceded = 0;

        for(int i = 0; i < teams[*teamCount].memberCount; i++){
            fscanf(file,"%s %d %s %d %d %d",
            teams[*teamCount].members[i].playerName,
            &teams[*teamCount].members[i].playerNumber,
            teams[*teamCount].members[i].playerPosition,
            &teams[*teamCount].members[i].goalsScored,
            &teams[*teamCount].members[i].goalsConceded,
            &teams[*teamCount].members[i].minutesPlayed
            );
        }


        (*teamCount)++;

       
    }

    fclose(file);
    printf("Team information loaded from file.\n");
}
/// implement function saveTeamsToFile
void saveTeamsToFile(struct Team teams[], int teamCount){
    FILE *file = fopen(fileName, "w");

    if(file == NULL){
        printf("File cannot be opened.\n");
        return;
    }

    printf("%d", teamCount);

    for(int i=0; i< teamCount; i++){
        fprintf(file,"%s %d\n",teams[i].teamName,teams[i].memberCount);
        for(int j = 0; j < teams[i].memberCount; j++){
            struct Player player = teams[i].members[j];
            fprintf(file,"%s %d %s %d %d %d\n",player.playerName,player.playerNumber,player.playerPosition,player.goalsScored,player.goalsConceded,player.minutesPlayed);
        }
    }
    fclose(file);
    printf("Team informations saved successfully.\n");

}
/// implement function addPlayerToTeam
void addPlayerToTeam(struct Team *team, struct Player *player){

    if(team -> memberCount > MAX_PLAYERS){
        printf("No slots available in the team.\n");
        return;
    }

    team->members[team->memberCount] = *player;
    team->memberCount++;

    
    team -> totalGoals += player -> goalsScored;
    team -> totalGoalsConceded += player -> goalsConceded;
 

    printf("Player added to the team successfully.\n");
}

/// implement function displayPlayer
void displayPlayer(struct Player *player){
    printf("%s\t%d\t%s\t%10d\t%10d\t%10d\n", player->playerName,player->playerNumber,player->playerPosition,player->goalsScored,player->goalsConceded,player->minutesPlayed);
}

/// implement function displayTeam
void displayTeam(struct Team *team){
    printf("Team Name: %s\n", team -> teamName);
    printf("Number of players: %d\n", team -> memberCount);
    printf("====================================================================\n");
    printf("Name\tNumber\tPosition\tGoals scored\tGoals Conceded\tMinutes Played\n");
    for(int i = 0; i < team -> memberCount; i++){
        displayPlayer(&team -> members[i]);
    }
    printf("\n");
}

/// implement function updateTeamInfo
void updatePlayerInfo(struct Player *player){
    int choice = 0;
    printf("Player menu:\n1.Update Name\n2.Update Number\n3.Update Goals Scored/Conceded\n4.Update Minutes Played\n5.Cancel\n");
    printf("Enter your choice: ");
    scanf("%d",&choice);

    switch(choice){
        case 1:
            printf("Enter New Name: ");
            scanf("%s", player->playerName);
            break;
        case 2:
            printf("Player Number: ");
            scanf("%d", &player->playerNumber);
            break;
        case 3:
            if(strcmp(player->playerPosition,"MID")==0 || strcmp(player->playerPosition,"FWD")==0){
                printf("Enter Goals Scored: ");
                scanf("%d", &player -> goalsScored);
            }else{
                printf("Enter Goals Conceded: ");
                scanf("%d", &player -> goalsConceded);
            }
            break;
        case 4:
            printf("Minutes Played: ");
            scanf("%d", &player->minutesPlayed);
            break;
        case 5:
            break;
    }

    
}

void updateTeamInfo(struct Team *team){
    int choice = 0;
    int playerChoice = 0;

    printf("Team menu:\n1.Update Team Name\n2.Update Player Info\n3.Cancel\n");
    printf("Enter your choice: ");
    scanf("%d",&choice);

    switch(choice){
        case 1:
            printf("Enter New Team Name: ");
            scanf("%s",team -> teamName);
            break;
        case 2:
            printf("Select player:\n");
            for(int i = 0; i < team -> memberCount; i++){
                printf("%d.%s\n",i+1,team->members[i].playerName);
            }
            printf("Enter your choice: ");
            scanf("%d",&playerChoice);
            updatePlayerInfo(&team -> members[playerChoice-1]);

            for(int i = 0; i < team -> memberCount; i++){
                team -> totalGoals += team -> members[playerChoice-1].goalsScored;
                team -> totalGoalsConceded += team -> members[playerChoice-1].goalsConceded;
            }

            break;
        case 3:
            break;
    }
}

/// implement function displayStatistics
void updateStatistics(struct Team teams[], int teamsCount){
    int mostGoalScoredTeam = 0;
    int leastGoalConcededTeam = 0;
    int mostMinsPlayedPlayer = 0;
    int mostMinsPlayedPlayerTeam = 0;

    for(int i = 0; i < teamsCount; i++){
        int totalGoals = 0;
        int totalGoalsConceded = 0;
    
        for(int j = 0; j < teams[i].memberCount; j++){
            totalGoals += teams[i].members[j].goalsScored;
            totalGoalsConceded += teams[i].members[j].goalsConceded;

            if(teams[i].members[j].minutesPlayed > teams[mostMinsPlayedPlayerTeam].members[mostMinsPlayedPlayer].minutesPlayed){
                mostMinsPlayedPlayer = j;
                mostMinsPlayedPlayerTeam = i;
            }
        }

        teams[i].totalGoals = totalGoals;
        teams[i].totalGoalsConceded = totalGoalsConceded;

        if(totalGoals > teams[mostGoalScoredTeam].totalGoals){
            mostGoalScoredTeam = i;
        }

        if(totalGoalsConceded < teams[leastGoalConcededTeam].totalGoalsConceded){
            leastGoalConcededTeam = i;
        }
       
    }

    FILE *file = fopen(statsFile,"w");

    fprintf(file,"Most goals scored: %s %d\nLeast goals conceded: %s %d\nMost minutes played: %s %s %d", 
    teams[mostGoalScoredTeam].teamName, 
    teams[mostGoalScoredTeam].totalGoals, 
    teams[leastGoalConcededTeam].teamName, 
    teams[leastGoalConcededTeam].totalGoalsConceded,
    teams[mostMinsPlayedPlayerTeam].members[mostMinsPlayedPlayer].playerName,
    teams[mostMinsPlayedPlayerTeam].teamName,
    teams[mostMinsPlayedPlayerTeam].members[mostMinsPlayedPlayer].minutesPlayed
    );

    fclose(file);
}

void showStats(struct Team teams[], int teamsCount){
    FILE *file = fopen(statsFile,"r");
    int c;

    printf("\n\n");
    while(1) {
      c = fgetc(file);
      if( feof(file) ) {
         break;
      }
      printf("%c", c);
   }
   printf("\n\n");

   fclose(file);
}


void chooseTeam(struct Team teams[],int teamCount, int *team){
    printf("\nChoose a team:\n");

    for(int i = 0; i < teamCount; i++){
        printf("%d.%s\n",i+1,teams[i].teamName);
    }

    printf("\nTeam: ");
    scanf("%d",&*team);
            
    printf("\nProceeding...\n\n");
}

int main() {
    struct Team footballTeams[MAX_TEAMS];
    int numTeams = 0;
    /// Load teams from file
    loadTeamsFromFile(footballTeams, &numTeams);
    updateStatistics(footballTeams,numTeams);

    int choice;
    while(1){
        printf("----------------------------------------------------------------------\n");
        printf("----------Welcome to Football Management System (FMS)-----------------\n");
        printf("----------------------------------------------------------------------\n\n");
        printf("Menu:\n1.Add a player\n2.Display Team information\n3.Update team information\n4.Show Statistics\n5.Save Team\n6.Exit\n");
        printf("Enter your choice: ");
        scanf("%d",&choice);

        int selectedTeam = 0;
        struct Player player;

        switch(choice){
        case 1:
            chooseTeam(footballTeams, numTeams, &selectedTeam);
            /// Add players to a team by taking player information as user input   
            printf("\nPlayer Name: ");
            scanf("%s",player.playerName);
            printf("\nPlayer Number: ");
            scanf("%d",&player.playerNumber);
            printf("\nPlayer Position(FWD/MID/DEF): ");
            scanf("%s",player.playerPosition);

            if(strcmp(player.playerPosition,"DEF")==0){
                printf("\nGoals Conceded: ");
                scanf("%d",&player.goalsConceded);
                player.goalsScored = 0;
            } else{
                printf("\nGoals Scored: ");
                scanf("%d",&player.goalsScored);
                player.goalsConceded = 0;
            }
           
            printf("\nMinutes Played: ");
            scanf("%d",&player.minutesPlayed);

            /// call the function addPlayerToTeam(team,player)
            addPlayerToTeam(&footballTeams[selectedTeam-1],&player);
            updateStatistics(footballTeams,numTeams);
            break;
        case 2:
            /// Display team information
            chooseTeam(footballTeams,numTeams,&selectedTeam);
            displayTeam(&footballTeams[selectedTeam-1]);
            break;
        case 3:
            /// Update Team information
            chooseTeam(footballTeams,numTeams,&selectedTeam);
            updateTeamInfo(&footballTeams[selectedTeam-1]);
            updateStatistics(footballTeams,numTeams);
            break;
        case 4:
            /// show statistics like Most goals scored, Most goals conceded by a team and most minutes played by a player
            showStats(footballTeams,numTeams);
            break;
        case 5:
            /// Save teams to file
            saveTeamsToFile(footballTeams, numTeams);
            break;
        case 6:
            printf("Thanks for using our System!!!");
            exit(0);
            break;
        }
    }
    return 0;
}
