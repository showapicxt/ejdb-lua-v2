/// Sample records put/query example
#include <ejdb2/ejdb2.h>

#define CHECK(rc_)          \
  if (rc_) {                 \
    iwlog_ecode_error3(rc_); \
    return 1;                \
  }

static iwrc documents_visitor(EJDB_EXEC *ctx, const EJDB_DOC doc, int64_t *step) {
  // Print document to stderr
  return jbl_as_json(doc->raw, jbl_fstream_json_printer, stderr, JBL_PRINT_PRETTY);
}

int main() {

  EJDB_OPTS opts = {
      .kv = {
          .path = "e:/data/example.db",
          .oflags = IWKV_NO_TRIM_ON_CLOSE
      },
      .no_wal=false
  };
  EJDB db;     // EJDB2 storage handle
  int64_t id;  // Document id placeholder
  JQL q = 0;   // Query instance
  JBL jbl = 0; // Json document

  iwrc rc = ejdb_init();
  CHECK(rc);

  rc = ejdb_open(&opts, &db);
  CHECK(rc);

  for(int i=0;i<10000;i++){
    // First record
    printf("ejdb_put_new ------------------- %d \n",i);
    rc = jbl_from_json(&jbl, "{\"name\":\"Bianca\", \"age\":4}");
    RCGO(rc, finish);
    rc = ejdb_put_new(db, "parrots", jbl, &id);
    RCGO(rc, finish);
    jbl_destroy(&jbl);

    // Second record
    rc = jbl_from_json(&jbl, "{\"name\":\"Darko\", \"age\":8}");
    RCGO(rc, finish);
    rc = ejdb_put_new(db, "parrots", jbl, &id);
    RCGO(rc, finish);
    jbl_destroy(&jbl);


  }


  // Now execute query
  rc =  jql_create(&q, "parrots", "/*");
  RCGO(rc, finish);

  EJDB_EXEC ux = {
      .db = db,
      .q = q,
      .limit=3,
      .visitor = documents_visitor
  };
  rc = ejdb_exec(&ux); // Execute query
  CHECK(rc);

  finish:
    jql_destroy(&q);
    jbl_destroy(&jbl);
//    Sleep(12000);
    printf("\n do close --------------------- \n");
    ejdb_close(&db);
    return 0;
}