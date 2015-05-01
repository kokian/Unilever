#ifndef CONSTANTS
#define CONSTANTS

typedef enum {
    ENT_EMPLOYEE = 0,
    ENT_PILLARS,
    ENT_SKILL_LEVELS,
    ENT_SKILL_TYPES,
    ENT_SKILLS,
    ENT_COUNT
} ENTITY_NAME;

static const QString tableNames[2] = {"ul_employees", "ul_skills"};

// SQL
static const QString SQL_CREATE_EMLOYEES_TABLE = "CREATE TABLE ul_employees ( "
                           "id SERIAL PRIMARY KEY NOT NULL, "
                           "name_leg TEXT, "
                           "job_title TEXT, "
                           "position_title TEXT, "
                           "department TEXT, "
                           "hire_date DATE, "
                           "code TEXT, "
                           "name TEXT, "
                           "reported TEXT "
                           ");";

static const QString SQL_CREATE_PILLARS_TABLE = "CREATE TABLE ul_pillars ( "
                           "id SERIAL PRIMARY KEY NOT NULL, "
                           "name TEXT "
                           ");";

static const QString SQL_CREATE_SKILL_LEVELS_TABLE = "CREATE TABLE ul_skill_levels ( "
                           "id SERIAL PRIMARY KEY NOT NULL, "
                           "name TEXT "
                           ");";

static const QString SQL_CREATE_SKILL_TYPES_TABLE = "CREATE TABLE ul_skill_types ( "
                           "id SERIAL PRIMARY KEY NOT NULL, "
                           "name TEXT "
                           ");";

static const QString SQL_CREATE_SKILLS_TABLE = "CREATE TABLE ul_skills ( "
                           "id SERIAL PRIMARY KEY NOT NULL, "
                           "number INTEGER, "
                           "level_id INTEGER REFERENCES ul_skill_levels (id), "
                           "type_id INTEGER REFERENCES ul_skill_types (id), "
                           "description TEXT, "
                           "pillar_id INTEGER REFERENCES ul_pillars (id), "
                           "production_process_engineer TEXT, "
                           "shift_leader TEXT, "
                           "logistic_engineer TEXT, "
                           "technician TEXT, "
                           "quality_engineer TEXT, "
                           "planning_engineer TEXT, "
                           "suit TEXT, "
                           "she_engineer TEXT, "
                           "env_engineer TEXT, "
                           "training_coordinator TEXT, "
                           "finance_specialist TEXT, "
                           "man_ex TEXT "
                           ");";

static const QString SQL_CREATE_ROLES_TABLE = "CREATE TABLE ul_roles ( "
                           "id SERIAL PRIMARY KEY NOT NULL, "
                           "name TEXT "
                           ");";

static const QString SQL_CREATE_TRAINING_PLANS_TABLE = "CREATE TABLE ul_training_plans ( "
                           "id SERIAL PRIMARY KEY NOT NULL, "
                           "code TEXT, "
                           "name TEXT, "
                           "group_id INTEGER, "
                           "training_subject TEXT, "
                           "target TEXT, "
                           "actual_status TEXT, "
                           "trainer TEXT "
                           ");";


static const QString SQL_CREATE_SKILLS_X_GROUPS_TABLE = "CREATE TABLE ul_skills_x_groups ( "
                           "id SERIAL PRIMARY KEY NOT NULL, "
                           "name_leg TEXT, "
                           "grade TEXT, "
                           "job_title TEXT, "
                           "position_title TEXT, "
                           "department TEXT, "
                           "hire_date DATE, "
                           "code TEXT, "
                           "name TEXT "
                           ");";



#endif // CONSTANTS

