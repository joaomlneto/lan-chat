/*
 * Database wrapper library
 *
 *
 */

#ifndef _DB_H_
#define _DB_H_

#include <xlcp_proto.h>

#define DB_SELECT_DATABASE MYSQL

#if DB_SELECT_DATABASE==MYSQL
#include "db_mysql.h"
#endif

/*
 * Database structures
 */
typedef struct {
	int id;
	char username[MAX_USERNAME_LENGTH];
	char password[MAX_PASSWORD_LENGTH];
	char name[MAX_NAME_LENGTH];
	char nick[MAX_NICK_LENGTH];
	char clan[MAX_CLAN_LENGTH];
	char phone[MAX_PHONE_LENGTH];
	char email[MAX_EMAIL_LENGTH];
	unsigned short int gender;
	char birthdate[8];
} db_xlcom_user_t;

typedef struct {
	int id;
	int user;
	char address[40];
	int request_type;
	char* request_details;
	char time[8];
	int result;
} db_xlcom_log_t;

/*
 * Database specific functions
 */
db_t* db_init();
db_t* db_connect(db_t* connection, char* host, unsigned int port, char* user, char* pass, char* db_name);
void db_disconnect(db_t* connection);

void db_query(db_t* connection, char* query);
db_result_t* db_get_result(db_t* connection);
db_row_t db_get_row(db_result_t* result);
void db_free_result(db_result_t* result);

db_xlcom_user_t db_row2user(db_row_t row);
db_xlcom_log_t db_row2log(db_row_t row);

/*
 * SQL query functions
 */
void db_query_add_user(db_t* db, db_xlcom_user_t* user);
void db_query_get_users(db_t* db);
void db_query_get_user_by_id(db_t* db, int id);
void db_query_get_user_by_auth(db_t* db, char* user, char* pass);

void db_query_add_log(db_xlcom_log_t* log);
void db_query_get_logs(db_t* db);
void db_query_get_log_by_id(db_t* db, int id);

#endif
