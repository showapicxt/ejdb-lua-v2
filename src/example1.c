/// Sample records put/query example
#include <ejdb2/ejdb2.h>

#define CHECK(rc_)                                                                                                     \
  if (rc_) {                                                                                                           \
    iwlog_ecode_error3(rc_);                                                                                           \
    return 1;                                                                                                          \
  }

static iwrc documents_visitor(EJDB_EXEC* ctx, const EJDB_DOC doc, int64_t* step) {
  // Print document to stderr
  if (doc->node) {
    return   jbn_as_json(doc->node, jbl_fstream_json_printer, stderr, 0);
  } else {
      return jbl_as_json(doc->raw, jbl_fstream_json_printer, stderr, JBL_PRINT_PRETTY);
  }


}

int mytest() {
  EJDB_OPTS opts = {.kv = {.path = "c:/db.jb", .oflags = IWKV_NO_TRIM_ON_CLOSE}, .no_wal = false};
  EJDB db;     // EJDB2 storage handle
  int64_t id;  // Document id placeholder
  JQL q = 0;   // Query instance
  JBL jbl = 0; // Json document
  printf("aaaaaaaaaaaaaaaaaaaa    \n");

  iwrc rc = ejdb_init();

  rc = ejdb_open(&opts, &db);
  //  for(int i=0;i<100000;i++){
  //    // First record
  //
  //    rc = jbl_from_json(&jbl, "{\"name\":\"Bianca\", \"age\":4}");
  //    rc = ejdb_put_new(db, "docs", jbl, &id);
  //    if(i%10000==0){
  //      printf("ejdb_put_new ------------------- %d \n",id);
  //    }
  //    jbl_destroy(&jbl);
  //
  //  }
  //  jql_destroy(&q);
  //  jbl_destroy(&jbl);
  //  printf("\n do close --------------------- \n");
  //  ejdb_close(&db);

  //  ejdb_open(&opts, &db);

  jql_create(&q, "docs", "/* |/name ");
  EJDB_EXEC ux = {.db = db, .q = q, .limit = 11, .visitor = documents_visitor};
  //  jql_set_str(q, "age", 0, "/{age}");
  //  jql_set_str(q, 0, 0, "{age}"); // Set positional string query parameter

  int a = ejdb_exec(&ux); // Execute query
  jql_destroy(&q);
  jbl_destroy(&jbl);
  printf("\n do close --------------------- %d \n", a);
  ejdb_close(&db);

  return 0;
}

long long get_usec() {
  struct timeval now;
  gettimeofday(&now, NULL);

  return (long long)now.tv_sec * 1000 + (long long)now.tv_usec / 1000;
}

void ejdb_test1_1() {
  EJDB_OPTS opts = {.kv = {.path = "ejdb_test1_1.db", .oflags = IWKV_TRUNC}, .no_wal = true};
  EJDB db;
  JBL meta, jbl;
  iwrc rc = ejdb_open(&opts, &db);
  rc = ejdb_ensure_collection(db, "foo");

  // Meta: {
  //  "version": "2.0.0",
  //  "file": "ejdb_test1_1.db",
  //  "size": 8192,
  //  "collections": [
  //    {
  //      "name": "foo",
  //      "dbid": 2
  //    }
  //  ]
  //}
  rc = ejdb_get_meta(db, &meta);

  rc = jbl_at(meta, "/collections", &jbl);
  jbl_as_json(meta, jbl_fstream_json_printer, stderr, JBL_PRINT_PRETTY);

  printf("\n  ---------------------  \n");

  jbl_destroy(&jbl);

  rc = jbl_at(meta, "/collections/0/name", &jbl);
  jbl_destroy(&jbl);
  jbl_destroy(&meta);

  rc = ejdb_close(&db);
  if (1 == 1) {
    return;
  }
  // Now reopen database then load collection
  opts.kv.oflags &= ~IWKV_TRUNC;
  rc = ejdb_open(&opts, &db);
  rc = ejdb_remove_collection(db, "foo");

  rc = ejdb_get_meta(db, &meta);
  jbl_as_json(meta, jbl_fstream_json_printer, stderr, JBL_PRINT_PRETTY);

  rc = jbl_at(meta, "/collections/0/name", &jbl); // No collections

  jbl_destroy(&meta);

  rc = ejdb_close(&db);
}

void update_test() {
  EJDB_OPTS opts = {.kv = {.path = "ejdb_test1_3.db", .oflags = IWKV_TRUNC}, .no_wal = true};
  EJDB db;
  JBL jbl;
  int64_t id = 0;

  iwrc rc = ejdb_open(&opts, &db);
  jbl_from_json(&jbl, "{\"name\":\"Bianca\", \"age\":4}");
  rc = ejdb_put_new(db, "c1", jbl, &id);
  //  jbl_from_json(&jbl, "{\"name\":\"22222\"}");

  rc = ejdb_patch(db, "c1", "{\"ss\":\"2223322\"}", id);
  if (rc) {
    iwlog_ecode_error3(rc);
  }
  printf("\n  ---------------------  \n");

  // Now check the result
  rc = ejdb_get(db, "c1", id, &jbl);
  jbl_as_json(jbl, jbl_fstream_json_printer, stderr, JBL_PRINT_PRETTY);

  IWXSTR* xstr = iwxstr_new();

  rc = jbl_as_json(jbl, jbl_xstr_json_printer, xstr, 0);

  rc = ejdb_close(&db);
  iwxstr_destroy(xstr);
  jbl_destroy(&jbl);
}



void  query_test() {
  EJDB_OPTS opts = {.kv = {.path = "ejdb_test1_3.db", .oflags = IWKV_TRUNC}, .no_wal = true};
  EJDB db;
  JBL jbl;
  int64_t id = 0;

  iwrc rc = ejdb_open(&opts, &db);
  jbl_from_json(&jbl, "{\"name\":\"Bianca\", \"age\":4}");
  rc = ejdb_put_new(db, "c1", jbl, &id);
  rc = ejdb_put_new(db, "c1", jbl, &id);


  rc = ejdb_patch(db, "c1", "{\"ss\":\"2223322\"}", id);
  if (rc) {
    iwlog_ecode_error3(rc);
  }
  printf("\n  ---------------------  \n");
  JQL q = 0;   // Query instance
  jql_create(&q, "c1", "/* | /{name,age} ");

  EJDB_LIST list = 0;
  ejdb_list4(db, q, 1, 0, &list);

  int i=0;
  for (EJDB_DOC doc = list->first; doc; doc = doc->next, ++i) {
    printf("\n do 1111111111 --------------------- %d \n");
//    jbl_as_json(doc->raw, jbl_fstream_json_printer, stderr, JBL_PRINT_PRETTY);
    jbn_as_json(doc->node, jbl_fstream_json_printer, stderr, 0);
    for (JBL_NODE n = doc->node->child; n; n = n->next) {

      char *key=n->key;
      iwlog_info("\n ddddddddddddddd  %s " ,key);
//      n = n->child;
      iwlog_info("\n dddddddddddddddssss  %s " ,n->vptr);

      char *value=n->vptr;

//      iwlog_info("eeeeeeeeeeeeeeeeeeeeeeeee  %s    %s ",key,value);
    }


  }
  ejdb_list_destroy(&list);

  jql_destroy(&q);
  jbl_destroy(&jbl);
  printf("\n do close --------------------- %d \n");
  ejdb_close(&db);
}





int main() {
  //  printf("%llu usec\n", get_usec());
  //  printf("%d usec\n", sizeof(int*) * 8);
  query_test();
//  mytest();
}