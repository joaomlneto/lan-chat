#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <mysql.h>
#include "db.h"
#include "db_mysql.h"

void db_error_print(db_t* db) {
	printf("[db_mysql] Error: [%u] %s\n", mysql_errno(db), mysql_error(db));
}

db_t* db_init() {
	MYSQL *db = mysql_init(NULL);
	if (db == NULL) {
		db_error_print(db);
		return NULL;
	}
	// Check if MySQL client is thread-safe
	if (mysql_thread_safe() == 0) {
		printf("[db_mysql] WARNING: Library is NOT thread-safe.\n");
	}
	return db;
}

db_t* db_connect(db_t* mysql, char* host, unsigned int port, char* user, char* pass, char* db) {
	db_t* result = mysql_real_connect(mysql, host, user, pass, db, port, NULL, 0);
	if (result == NULL) {
		db_error_print(mysql);
	}
	return result;
}

void db_query(db_t* db, char* query) {
	if (mysql_query(db, query) != 0) {
		db_error_print(db);
	}
}

void db_disconnect(db_t* connection) {
	mysql_close(connection);
}

db_result_t* db_get_result(db_t* connection) {
	return mysql_store_result(connection);
}

db_row_t db_get_row(db_result_t* result) {
	return mysql_fetch_row(result);
}

void db_free_result(db_result_t* result) {
	mysql_free_result(result);
}

db_xlcom_user_t db_row2user(db_row_t row) {
	db_xlcom_user_t user;
	user.id = atoi(row[0]);
	strncpy(user.username, row[1], 32);
	strncpy(user.password, row[2], 40);
	strncpy(user.name, row[3], 64);
	strncpy(user.nick, row[4], 32);
	strncpy(user.clan, row[5], 32);
	strncpy(user.phone, row[6], 16);
	strncpy(user.email, row[7], 128);
	user.gender = atoi(row[8]);
	strncpy(user.birthdate, row[9], 8);
	return user;
}

db_xlcom_log_t db_row2log(db_row_t row) {
	db_xlcom_log_t log;
	log.id = atoi(row[0]);
	log.user = atoi(row[1]);
	strncpy(log.address, row[2], 40);
	log.request_type = atoi(row[3]);
	log.request_details = malloc((strlen(row[4])+1)*sizeof(char));
	strcpy(log.request_details, row[4]);
	strncpy(log.time, row[5], 8);
	log.result = atoi(row[6]);
	return log;
}
