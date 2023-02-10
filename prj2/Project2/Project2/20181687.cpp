#include <stdio.h>
#include <string.h>
#include "mysql.h"

#pragma comment(lib, "libmysql.lib")

const char* host = "localhost";
const char* user = "root";
const char* pw = "mysql";
const char* db = "project";

MYSQL* connection = NULL;
MYSQL conn;
MYSQL_RES* sql_result;
MYSQL_ROW sql_row;

int t_n = 1010;

void type1() {
	int x = 0, t = 0, state = 0;
	while (1) {
		printf("-------- TYPE 1 --------\n\n");
		printf("** Assume the package shipped by USPS with tracking number X is reported to have been destroyed in an accident.Find the contact information for the customer.**\n");
		printf("Enter tracking number X(Enter 0 if you want to quit):");
		scanf_s("%d", &x);

		if (x == 0) break;
		char q1[500];
		sprintf(q1, "SELECT customer_ID, comp_name, comp_phone  FROM shipping_info natural join ship_comp WHERE tracking_num = %d and comp_name = 'USPS' ", x);
		state = 0;
		state = mysql_query(connection, q1);
		if (state == 0) {
			sql_result = mysql_store_result(connection);

			printf("========================================\n");
			printf("customer_ID     comp_name     comp_phone\n");
			printf("========================================\n");
			while ((sql_row = mysql_fetch_row(sql_result)) != NULL)
			{
				printf("%11s     %9s     %10s\n", sql_row[0], sql_row[1], sql_row[2]);
			}
			printf("\n");
			mysql_free_result(sql_result);
		}

		int t;
		printf("------- Subtypes in TYPE 1 -------\n\n");
		printf("\t1. TYPE 1-1.\n");
		printf("Which subtype?(Enter 0 if you want to quit): ");
		scanf_s("%d", &t);
		if (t == 0)break;

		else if (t == 1) {
			printf("\n----------- TYPE 1-1 -----------\n\n");
			printf("** Then find the contents of that shipment and create a new shipment of replacement items **\n");
			char update[500];
			state = 0;
			sprintf(update, "UPDATE shipping_info SET  tracking_num = %d, comp_name = 'DELI' WHERE tracking_num = %d", t_n, x);
			state = mysql_query(connection, update);

			char q1_1[500];
			sprintf(q1_1, "SELECT tracking_num, comp_name, comp_phone, customer_ID, product_ID FROM shipping_info natural join ship_comp WHERE tracking_num = % d", t_n);
			t_n++;
			state = 0;
			state = mysql_query(connection, q1_1);
			if (state == 0) {
				sql_result = mysql_store_result(connection);

				printf("=========================================================================\n");
				printf(" tracking_num     comp_name     comp_phone     customer_ID     product_ID\n");
				printf("=========================================================================\n");
				while ((sql_row = mysql_fetch_row(sql_result)) != NULL) {
					printf("%11s     %9s     %9s     %10s     %9s\n", sql_row[0], sql_row[1], sql_row[2], sql_row[3], sql_row[4]);
				}
				printf("\n");
				mysql_free_result(sql_result);
			}
		}
	}
	return;
}

void type2() {
	int state = 0;
	
	printf("-------- TYPE 2 --------\n\n");
	printf("** Find the customer who has bought the most (by price) in the past year. **\n");
	char q2[500];

	sprintf(q2, "SELECT customer_ID, sum(amount * price) as bill FROM purchase natural join product WHERE date > '2020-12-31' and date < '2022-01-01' GROUP BY customer_ID ORDER BY bill desc ");
	state = 0;
	state = mysql_query(connection, q2);

	if (state == 0) {
		sql_result = mysql_store_result(connection);
		printf("==============================\n");
		printf(" customer_ID     year     bill\n");
		printf("==============================\n");
		sql_row = mysql_fetch_row(sql_result);
		printf("%11s     2021     %4s\n", sql_row[0], sql_row[1]);
		printf("\n");
		mysql_free_result(sql_result);
	}
	while (1) {
		int t;
		printf("------- Subtypes in TYPE 2 -------\n\n");
		printf("\t1. TYPE 2-1.\n");
		printf("Which subtype?(Enter 0 if you want to quit): ");
		scanf_s("%d", &t);
		if (t == 0)break;

		else if (t == 1) {
			printf("\n----------- TYPE 2-1 -----------\n\n");
			printf("** Then find the product that the customer bought the most. **\n");
			char q2_1[500];
			sprintf(q2_1, "SELECT product_ID, sum(amount) as num FROM purchase natural join product GROUP BY product_ID ORDER BY num desc");
			state = 0;
			state = mysql_query(connection, q2_1);
			if (state == 0) {
				sql_result = mysql_store_result(connection);

				printf("======================\n");
				printf(" product_ID     amount\n");
				printf("======================\n");
				sql_row = mysql_fetch_row(sql_result);
				printf("%11s     %6s\n", sql_row[0], sql_row[1]);
				printf("\n");
				mysql_free_result(sql_result);
			}
		}
	}

	return;
}

void type3() {
	int x = 0, state = 0;
	printf("-------- TYPE 3 --------\n\n");
	printf("** Find all products sold in the past year. **\n");

	char q3[500];
	sprintf(q3, "SELECT product_ID, sum(amount) FROM sales_info natural join product  WHERE date > '2020-12-31' and date < '2022-01-01' GROUP BY product_ID");
	state = 0;
	state = mysql_query(connection, q3);
	if (state == 0) {
		sql_result = mysql_store_result(connection);

		printf("======================\n");
		printf(" product_ID     amount\n");
		printf("======================\n");
		while ((sql_row = mysql_fetch_row(sql_result)) != NULL) {
			printf("%10s     %5s\n", sql_row[0], sql_row[1]);
		}
		printf("\n");
		mysql_free_result(sql_result);
	}

	int t;
	while (1) {
		printf("------- Subtypes in TYPE 3 -------\n\n");
		printf("\t1. TYPE 3-1.\n");
		printf("\t1. TYPE 3-2.\n");
		printf("Which subtype?(Enter 0 if you want to quit): ");
		scanf_s("%d", &t);
		if (t == 0)break;

		else if (t == 1) {
			int k;
			printf("\n----------- TYPE 3-1 -----------\n\n");
			printf("** Then find the top k products by dollar-amount sold. **\n");
			printf("Which K? (Enter 0 if you want to quit) : ");
			scanf_s("%d", &k);
			char q3_1[500];
			sprintf(q3_1, "SELECT product_ID, (sum(amount) * price) as total FROM sales_info natural join product GROUP BY product_ID ORDER BY total desc");
			state = 0;
			state = mysql_query(connection, q3_1);
			if (state == 0) {
				sql_result = mysql_store_result(connection);

				printf("===============================\n");
				printf(" rank     product_ID     amount\n");
				printf("===============================\n");
				int rank = 1;
				while (rank < k + 1) {
					sql_row = mysql_fetch_row(sql_result);
					printf("%4d     %10s     %6s\n", rank, sql_row[0], sql_row[1]);
					rank++;
				}
				printf("\n");
				mysql_free_result(sql_result);
			}
		}

		else if (t == 2) {
			printf("\n----------- TYPE 3-2 -----------\n\n");
			printf("** And then find the top 10%% products by dollar-amount sold. **\n");
			char count[500];
			int c = 0;
			sprintf(count, "SELECT product_ID, (sum(amount) * price) as total FROM sales_info natural join product GROUP BY product_ID ORDER BY total desc");
			state = 0;
			state = mysql_query(connection, count);
			if (state == 0) {
				sql_result = mysql_store_result(connection);
				while ((sql_row = mysql_fetch_row(sql_result)) != NULL) 
					c++;
				mysql_free_result(sql_result);
			}

			char q3_2[500];
			sprintf(q3_2, "SELECT product_ID, (sum(amount) * price) as total FROM sales_info natural join product GROUP BY product_ID ORDER BY total desc");
			state = 0;
			state = mysql_query(connection, q3_2);
			if (state == 0) {
				sql_result = mysql_store_result(connection);

				printf("===============================\n");
				printf(" rank     product_ID     amount\n");
				printf("===============================\n");
				int rank = 1;
				while (rank <= (int)(c *0.1)) {
					sql_row = mysql_fetch_row(sql_result);
					printf("%4d     %10s     %6s\n", rank, sql_row[0], sql_row[1]);
					rank++;
				}
				printf("\n");
				mysql_free_result(sql_result);
			}
		}
	}

	return;
}

void type4() {
	int state = 0;
	printf("-------- TYPE 4 --------\n\n");
	printf("**  Find all products by unit sales in the past year. **\n");

	char q4[500];
	sprintf(q4, "SELECT package, sum(amount) FROM sales_info natural join product WHERE date > '2020-12-31' and date < '2022-01-01' GROUP BY package");
	state = 0;
	state = mysql_query(connection, q4);
	if (state == 0) {
		sql_result = mysql_store_result(connection);

		printf("=======================\n");
		printf(" unit            amount\n");
		printf("=======================\n");
		while ((sql_row = mysql_fetch_row(sql_result)) != NULL) {
			printf("%-15s     %s\n", sql_row[0], sql_row[1]);
		}
		printf("\n");
		mysql_free_result(sql_result);
	}

	int t;
	while (1) {
		printf("------- Subtypes in TYPE 4 -------\n\n");
		printf("\t1. TYPE 4-1.\n");
		printf("\t1. TYPE 4-2.\n");
		printf("Which subtype?(Enter 0 if you want to quit): ");
		scanf_s("%d", &t);
		if (t == 0)break;

		else if (t == 1) {
			int k;
			printf("\n----------- TYPE 4-1 -----------\n\n");
			printf("** Then find the top k products by unit sales. **\n");
			printf("Which K? (Enter 0 if you want to quit) : ");
			scanf_s("%d", &k);
			char q4_1[500];
			sprintf(q4_1, "SELECT package, sum(amount) as total FROM sales_info natural join product GROUP BY package ORDER BY total desc");
			state = 0;
			state = mysql_query(connection, q4_1);
			if (state == 0) {
				sql_result = mysql_store_result(connection);

				printf("===============================\n");
				printf(" rank     product_ID     amount\n");
				printf("===============================\n");
				int rank = 1;
				while (rank < k + 1) {
					sql_row = mysql_fetch_row(sql_result);
					printf("%4d     %-15s     %s\n", rank, sql_row[0], sql_row[1]);
					rank++;
				}
				printf("\n");
				mysql_free_result(sql_result);
			}
		}

		else if (t == 2) {
			printf("\n----------- TYPE 4-2 -----------\n\n");
			printf("**  And then find the top 10%% products by unit sales. **\n");
			char count[500];
			int c = 0;
			sprintf(count, "SELECT package, sum(amount) as total FROM sales_info natural join product GROUP BY package ORDER BY total desc");
			state = 0;
			state = mysql_query(connection, count);
			if (state == 0) {
				sql_result = mysql_store_result(connection);
				while ((sql_row = mysql_fetch_row(sql_result)) != NULL)
					c++;
				mysql_free_result(sql_result);
			}

			char q4_2[500];
			sprintf(q4_2, "SELECT package, sum(amount) as total FROM sales_info natural join product GROUP BY package ORDER BY total desc");
			state = 0;
			state = mysql_query(connection, q4_2);
			if (state == 0) {
				sql_result = mysql_store_result(connection);

				printf("===============================\n");
				printf(" rank     product_ID     amount\n");
				printf("===============================\n");
				int rank = 1;
				while (rank <= (int)(c * 0.1)) {
					sql_row = mysql_fetch_row(sql_result);
					printf("%4d     %10s     %6s\n", rank, sql_row[0], sql_row[1]);
					rank++;
				}
				printf("\n");
				mysql_free_result(sql_result);
			}
		}
	}

	return;
}

void type5() {
	int x = 0, state = 0;
	while (1) {
		printf("-------- TYPE 5 --------\n\n");
		printf("** Find those products that are out-of-stock at every store in California. **\n");
		char q5[500];
		sprintf(q5, "SELECT S_ID, product_ID, region, remain, M_ID FROM store WHERE region = 'California' and remain = '0'");
		state = 0;
		state = mysql_query(connection, q5);
		if (state == 0) {
			sql_result = mysql_store_result(connection);

			printf("===================================================\n");
			printf(" S_ID     product_ID     region     remain     M_ID\n");
			printf("===================================================\n");
			while ((sql_row = mysql_fetch_row(sql_result)) != NULL) {
				printf("%4s     %9s     %5s     %3s     %4s\n", sql_row[0], sql_row[1], sql_row[2], sql_row[3], sql_row[4]);
			}
			printf("\n");
			mysql_free_result(sql_result);
		}

		printf("Enter 0 if you want to quit(if not, enter anything you want):");
		scanf_s("%d", &x);

		if (x == 0)break;
	}
	return;
}

void type6() {
	int x = 0, state = 0;
	while (1) {
		printf("-------- TYPE 6 --------\n\n");
		printf("** Find those packages that were not delivered within the promised time. **\n");
		
		char q6[500];
		sprintf(q6, "SELECT tracking_num, schdule_date, arrival_date, comp_name, comp_phone, customer_ID, product_ID FROM shipping_info natural join ship_comp WHERE schdule_date <> arrival_date");
		state = 0;
		state = mysql_query(connection, q6);
		if (state == 0) {
			sql_result = mysql_store_result(connection);

			printf("===========================================================================================================\n");
			printf(" tracking_num     schdule_date     arrival_date     comp_name     comp_phone     customer_ID     product_ID\n");
			printf("===========================================================================================================\n");
			while ((sql_row = mysql_fetch_row(sql_result)) != NULL) {
				printf("%11s     %12s     %12s     %9s     %9s     %10s     %9s\n", sql_row[0], sql_row[1], sql_row[2], sql_row[3], sql_row[4], sql_row[5], sql_row[6]);
			}
			printf("\n");
			mysql_free_result(sql_result);
		}

		printf("Enter 0 if you want to quit(if not, enter anything you want):");
		scanf_s("%d", &x);

		if (x == 0)break;
	}
	return;
}

void type7() {
	int x = 0, state = 0;
	while (1) {
		printf("-------- TYPE 7 --------\n\n");
		printf("** Generate the bill for each customer for the past month. **\n");
	
		char q7[500];
		sprintf(q7, "SELECT customer_ID, sum(amount * price) FROM purchase natural join product WHERE date > '2022-04-30' and date < '2022-06-01' GROUP BY customer_ID");
		state = 0;
		state = mysql_query(connection, q7);
		if (state == 0) {
			sql_result = mysql_store_result(connection);

			printf("==============================\n");
			printf(" customer_ID     month     bill\n");
			printf("==============================\n");
			while ((sql_row = mysql_fetch_row(sql_result)) != NULL) {
				printf("%11s     2022-05    %s\n", sql_row[0], sql_row[1]);
			}
			printf("\n");
			mysql_free_result(sql_result);
		}
		printf("Enter 0 if you want to quit(if not, enter anything you want):");
		scanf_s("%d", &x);

		if (x == 0)break;
	}
	return;
}

int main(void) {
	if (mysql_init(&conn) == NULL)
		printf("mysql_init() error!");
	
	connection = mysql_real_connect(&conn, host, user, pw, db, 3306, (const char*)NULL, 0);
	
	if (connection == NULL){
		printf("%d ERROR : %s\n", mysql_errno(&conn), mysql_error(&conn));
		return 1;
	}
	
	else{
		printf("Connection Succeed\n");
		if (mysql_select_db(&conn, db)){
			printf("%d ERROR : %s\n", mysql_errno(&conn), mysql_error(&conn));
			return 1;
		}
		
		FILE* fp = fopen("20181687.txt", "r");
		char temp[500];
		while (1) {
			if (feof(fp)) break;
			fgets(temp, 500, fp);
			mysql_query(connection, temp);
		}
		if (temp != NULL)
			mysql_query(connection, temp);
		fclose(fp);

		int n;

		while (1) {
			printf("-------- SELECT QUERY TYPES --------\n\n");
			printf("\t1. TYPE 1\n");
			printf("\t2. TYPE 2\n");
			printf("\t3. TYPE 3\n");
			printf("\t4. TYPE 4\n");
			printf("\t5. TYPE 5\n");
			printf("\t6. TYPE 6\n");
			printf("\t7. TYPE 7\n");
			printf("\t0. QUIT\n");
			printf("Enter number(0 to 7): ");
			scanf_s("%d", &n);
			if (n == 0) break;
			else if (n == 1) type1();
			else if (n == 2) type2();
			else if (n == 3) type3();
			else if (n == 4) type4();
			else if (n == 5) type5();
			else if (n == 6) type6();
			else if (n == 7) type7();
			else printf("Invalid number\n");
		}

		fp = fopen("20181687-2.txt", "r");
		while (1) {
			if (feof(fp)) break;
			fgets(temp, 500, fp);
			mysql_query(connection, temp);
		}
		fclose(fp);

		mysql_close(connection);
	}

	return 0;
}