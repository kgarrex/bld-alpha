
CREATE TABLE project(
  name TEXT NOT NULL
 ,masterRecordId INTEGER NOT NULL
);

CREATE TABLE build(
  projectId INTEGER NOT NULL
 ,buildTime INTEGER NOT NULL
 ,FOREIGN KEY(projectId) REFERENCES project(rowid)
);
