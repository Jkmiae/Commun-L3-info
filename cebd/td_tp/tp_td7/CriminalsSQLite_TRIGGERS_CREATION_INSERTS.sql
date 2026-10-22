DROP TABLE IF EXISTS Criminals_log;
DROP TABLE IF EXISTS Criminals;
DROP TABLE IF EXISTS Crimes;

CREATE TABLE IF NOT EXISTS Crimes (
    crimeId  INTEGER PRIMARY KEY,
    type     TEXT,
    location TEXT,
    year     INTEGER,
    CONSTRAINT Crimes_C1
            CHECK (type IN ('Heist', 'Hijack', 'Kidnap', 'Riots'))
);

CREATE TABLE IF NOT EXISTS Criminals (
    pseudo     TEXT PRIMARY KEY,
    name       TEXT,
    speciality TEXT,
    birthYear  INTEGER,
    sentence   INTEGER,
    crimeId    INTEGER NOT NULL,
    CONSTRAINT Criminals_C1 FOREIGN KEY (crimeId) REFERENCES Crimes(crimeId),
    constraint Criminals_C2 CHECK (birthYear > 1900)
);

CREATE TABLE IF NOT EXISTS Criminals_log(
    pseudo       TEXT,
    oldsentence  NUMBER(2),
    dateofchange DATE,
    CONSTRAINT Criminals_log_C1 PRIMARY KEY (pseudo, oldsentence),
    CONSTRAINT Criminals_log_C2 FOREIGN KEY (pseudo) REFERENCES Criminals (pseudo)
);

-- INSERTS OK
INSERT INTO Crimes (crimeId, type, location, year)
    VALUES (2019089, 'Riots', 'Gotham', 2019);
INSERT INTO Crimes (crimeId, type, location, year)
    VALUES (2017052, 'Heist', 'FNMT', 2017);
INSERT INTO Crimes (crimeId, type, location, year)
    VALUES (2003215, 'Heist', 'Antwerp', 2003);

INSERT INTO Criminals (pseudo, name, speciality, birthyear, sentence, crimeId)
    VALUES ('El Profesor', 'Sergio', 'Band Leader', 1983, 10, 2017052);
INSERT INTO Criminals (pseudo, name, speciality, birthyear, sentence, crimeId)
    VALUES ('Tokio', 'Silene', 'Robber', 1989, 17, 2017052);
INSERT INTO Criminals (pseudo, name, speciality, birthyear, sentence, crimeId)
    VALUES ('Joker', 'Arthur', 'Chemical Eng.', 1985, 10, 2019089);

-- TRIGGERS
-- Can be tested individually
DROP TRIGGER IF EXISTS Criminals_logging_OK;
CREATE TRIGGER Criminals_logging_OK
   BEFORE UPDATE OF sentence ON Criminals
   WHEN (OLD.sentence - NEW.sentence)>=2
BEGIN
   INSERT INTO Criminals_log VALUES (OLD.pseudo, OLD.sentence, DATE('now'));
END;


DROP TRIGGER IF EXISTS CriminalsNotIncreasingSentence;
CREATE TRIGGER IF NOT EXISTS  CriminalsNotIncreasingSentence
   BEFORE UPDATE OF sentence ON Criminals
WHEN EXISTS
   (SELECT *
   FROM Criminals C
   WHERE C.pseudo = NEW.pseudo AND NEW.sentence > OLD.sentence)
BEGIN
   SELECT RAISE (ABORT, 'Cannot increase a Criminal sentence');
END;

DROP TRIGGER IF EXISTS CriminalsAndCrimesDates;
CREATE TRIGGER IF NOT EXISTS  CriminalsAndCrimesDates
   BEFORE INSERT ON Criminals
WHEN EXISTS
   (SELECT *
   FROM Crimes C
   WHERE C.crimeId=NEW.crimeId AND NEW.birthYear > C.year)
BEGIN
   SELECT RAISE (ABORT, 'Cannot insert criminal with birthYear > Crime.year');
END;

DROP TRIGGER IF EXISTS OnlyOneLeader;

CREATE TRIGGER IF NOT EXISTS OnlyOneLeader
BEFORE INSERT ON Criminals
-- An alternative structure to the EXISTS one
WHEN  (SELECT COUNT(*)
       FROM Criminals C
       WHERE NEW.speciality= 'Band Leader' AND C.crimeID=NEW.crimeID) > 0
BEGIN
   SELECT RAISE (ABORT, 'Cannot have more than one leader in a band');
END;


-- TEST INSERTS
--NOK CrimeDate (2017) > birthYear (2019) if CriminalsAndCrimesDates TRIGGER created
INSERT INTO Criminals (pseudo, name, speciality, birthyear, sentence, crimeId)
    VALUES ('Toto', 'Phil', 'Robber', 2019, 10, 2017052);

--NOK if OnlyOneLeader TRIGGER created
INSERT INTO Criminals (pseudo, name, speciality, birthyear, sentence, crimeId)
    VALUES ('Titi', 'Phil', 'Band Leader', 2016, 10, 2017052);

-- Logging NOK If WHERE UNCOMMENTED in Criminals_logging_OK TRIGGER
-- Insert NOK If CriminalsNotIncreasingSentence TRIGGER created
UPDATE Criminals SET sentence=15 WHERE pseudo='El Profesor';


-- Not Logged because of the WHERE OLD.sentence - NEW.sentence < 2 but modified (UPDATE OK)
UPDATE Criminals SET sentence=9 WHERE pseudo='El Profesor';

-- Logged and modified (UPDATE OK)
UPDATE Criminals SET sentence=5 WHERE pseudo='El Profesor';
