/*
 * Database wrapper library
 *
 * db.c
 *
 * Implements functions to access 
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "db.h"

/*
 * SQL query functions
 */

void db_query_add_user(db_t* db, db_xlcom_user_t* user) {
	char query[1024];
	sprintf(query, "INSERT INTO xlcom_users ( `id`, `username`, `password`, `name`, `nick`, `clan`, `phone`, `email`, `gender`, `birthdate`)\
	VALUES (NULL, '%s', SHA1('%s'), '%s', '%s', '%s', '%s', '%s', %d, '%s');",
	user->username, user->password, user->name, user->nick, user->clan, user->phone, user->email, user->gender, user->birthdate);
	db_query(db, query);
}

void db_query_get_users(db_t* db) {
	db_query(db, "SELECT * FROM xlcom_users");
}

void db_query_get_user_by_id(db_t* db, int id) {
	char query[1024];
	sprintf(query, "SELECT * FROM xlcom_users WHERE id=%d", id);
	db_query(db, query);
}

void db_query_get_user_by_auth(db_t* db, char* user, char* pass) {
	char query[1024];
	sprintf(query, "SELECT * FROM xlcom_users WHERE username='%s' AND password='%s'", user, pass);
	db_query(db, query);
}

void db_query_get_logs(db_t* db) {
	db_query(db, "SELECT * FROM xlcom_logs");
}

void db_query_get_log_by_id(db_t* db, int id) {
	char query[1024];
	sprintf(query, "SELECT * FROM xlcom_logs WHERE id=%d", id);
	db_query(db, query);
}
