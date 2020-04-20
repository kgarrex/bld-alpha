

CREATE TABLE projectType(
  name TEXT NOT NULL
 ,templatePath TEXT NOT NULL
);

INSERT INTO projectType (name, templatePath)
VALUES
  ('CApp', '$.ProjectTemplates.CApp')
 ,('AndroidPackage', '$.ProjectTemplates.AndroidPackage');


/*
CREATE TABLE project(
  project_name TEXT NOT NULL
 ,project_version INTEGER NOT NULL
 ,project_target_arch INTEGER NULL
);
*/

--Flags: active, deleted

CREATE TABLE projects(
  name              TEXT NOT NULL
 ,projectTypeId     INTEGER NOT NULL
 ,createTime        INTEGER NOT NULL
 ,lastBuildTime     INTEGER NULL
 ,versionControlUrl TEXT NULL
 ,FOREIGN KEY(projectTypeId) REFERENCES project_type(rowid)
);
