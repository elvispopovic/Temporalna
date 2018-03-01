--
-- PostgreSQL database dump
--

-- Dumped from database version 10.1
-- Dumped by pg_dump version 10.1

SET statement_timeout = 0;
SET lock_timeout = 0;
SET idle_in_transaction_session_timeout = 0;
SET client_encoding = 'UTF8';
SET standard_conforming_strings = on;
SET check_function_bodies = false;
SET client_min_messages = warning;
SET row_security = off;

--
-- Name: plpgsql; Type: EXTENSION; Schema: -; Owner: 
--

CREATE EXTENSION IF NOT EXISTS plpgsql WITH SCHEMA pg_catalog;


--
-- Name: EXTENSION plpgsql; Type: COMMENT; Schema: -; Owner: 
--

COMMENT ON EXTENSION plpgsql IS 'PL/pgSQL procedural language';


SET search_path = public, pg_catalog;

--
-- Name: azuriranje_dobavljaca(); Type: FUNCTION; Schema: public; Owner: epc
--

CREATE FUNCTION azuriranje_dobavljaca() RETURNS trigger
    LANGUAGE plpgsql
    AS $$
DECLARE
 dubina INTEGER;
BEGIN
 dubina = pg_trigger_depth();
IF TG_OP='UPDATE' THEN
 IF dubina=3 OR dubina =2 THEN RETURN NEW; 
 ELSIF OLD.vrijeme_do='infinity'::TIMESTAMP THEN
  INSERT INTO dobavljaci(id,naziv,vrijeme_do,adresa,telefon,telefon2,"e-mail") 
  VALUES(NEW.id, NEW.naziv, 'infinity'::TIMESTAMP, NEW.adresa, NEW.telefon, NEW.telefon2, NEW."e-mail");
 END IF;
 RETURN NULL;
END IF;
IF TG_OP='INSERT' THEN
 UPDATE dobavljaci SET vrijeme_do=current_timestamp WHERE 
  id=NEW.id AND vrijeme_do='infinity'::TIMESTAMP;
  RETURN NEW;
END IF;
IF TG_OP='DELETE' THEN
 UPDATE dobavljaci SET vrijeme_do=current_timestamp WHERE
  id=OLD.id AND vrijeme_od = OLD.vrijeme_od AND 
  vrijeme_do='infinity'::TIMESTAMP;
  RETURN NULL;
END IF;
END;
$$;


ALTER FUNCTION public.azuriranje_dobavljaca() OWNER TO epc;

--
-- Name: azuriranje_materijala(); Type: FUNCTION; Schema: public; Owner: epc
--

CREATE FUNCTION azuriranje_materijala() RETURNS trigger
    LANGUAGE plpgsql
    AS $$
DECLARE
redak dobavljaci%ROWTYPE;
dubina INTEGER;
BEGIN
dubina = pg_trigger_depth();
IF TG_OP='UPDATE' THEN
 IF dubina=3 OR dubina =2 THEN RETURN NEW; 
 ELSIF OLD.vrijeme_do='infinity'::TIMESTAMP THEN
  INSERT INTO materijali(id,dobavljac, vrijeme_dobavljaca,vrijeme_do,naziv)
  VALUES(NEW.id, NEW.dobavljac, NEW.vrijeme_dobavljaca, 'infinity'::TIMESTAMP, NEW.naziv);  
 END IF;
  RETURN NULL;
END IF;
IF TG_OP='INSERT' THEN
 SELECT * INTO redak FROM dobavljaci WHERE id=NEW.dobavljac AND vrijeme_do='infinity'::TIMESTAMP;
 IF NOT FOUND THEN 
  RAISE EXCEPTION 'Ne postoji taj dobavljac.';
  RETURN NULL;
 END IF;

 UPDATE materijali SET vrijeme_do=current_timestamp WHERE 
   id=NEW.id AND vrijeme_do='infinity'::TIMESTAMP;
 NEW.vrijeme_dobavljaca = redak.vrijeme_od;
 RETURN NEW;
END IF;
IF TG_OP='DELETE' THEN
 UPDATE materijali SET vrijeme_do=current_timestamp WHERE
  id=OLD.id AND vrijeme_od = OLD.vrijeme_od AND 
  vrijeme_do='infinity'::TIMESTAMP;
  RETURN NULL;
END IF;
END;
$$;


ALTER FUNCTION public.azuriranje_materijala() OWNER TO epc;

--
-- Name: azuriranje_shim_zavojnice(); Type: FUNCTION; Schema: public; Owner: epc
--

CREATE FUNCTION azuriranje_shim_zavojnice() RETURNS trigger
    LANGUAGE plpgsql
    AS $$
DECLARE
redak1 dobavljaci%ROWTYPE;
redak2 materijali%ROWTYPE;
redak3 shim_zavojnice%ROWTYPE;
dubina INTEGER;
BEGIN
dubina = pg_trigger_depth();
IF TG_OP='UPDATE' THEN
 IF dubina=3 OR dubina =2 THEN RETURN NEW; 
 ELSIF OLD.vrijeme_do='infinity'::TIMESTAMP THEN
  INSERT INTO shim_zavojnice(id,dobavljac, vrijeme_dobavljaca,vrijeme_do,naziv, tip,max_struja,sparivanje,promjer,jakost)
  VALUES(NEW.id, NEW.dobavljac, NEW.vrijeme_dobavljaca, 'infinity'::TIMESTAMP, NEW.naziv, NEW.tip, NEW.max_struja, 
  NEW.sparivanje, NEW.promjer, NEW.jakost);
 END IF;
 RETURN NULL;
END IF;
IF TG_OP='INSERT' THEN
 SELECT * INTO redak2 FROM materijali WHERE id=NEW.id;
 IF FOUND THEN
  SELECT * INTO redak3 FROM ONLY shim_zavojnice WHERE id=NEW.id;
  IF NOT FOUND THEN
   SELECT max(id)+1 INTO NEW.id FROM materijali;
   RAISE NOTICE 'Novi id: %',NEW.id;
  END IF;
 END IF; 
 SELECT * INTO redak1 FROM dobavljaci WHERE id=NEW.dobavljac AND vrijeme_do='infinity'::TIMESTAMP;
 IF NOT FOUND THEN 
  RAISE EXCEPTION 'Ne postoji taj dobavljac.';
  RETURN NULL;
 END IF;

 UPDATE shim_zavojnice SET vrijeme_do=current_timestamp WHERE 
   id=NEW.id AND vrijeme_do='infinity'::TIMESTAMP;
  NEW.vrijeme_dobavljaca = redak1.vrijeme_od;
 RETURN NEW;
END IF;
IF TG_OP='DELETE' THEN
 UPDATE shim_zavojnice SET vrijeme_do=current_timestamp WHERE
  id=OLD.id AND vrijeme_od = OLD.vrijeme_od AND 
  vrijeme_do='infinity'::TIMESTAMP;
  RETURN NULL;
END IF;
END;
$$;


ALTER FUNCTION public.azuriranje_shim_zavojnice() OWNER TO epc;

--
-- Name: azuriranje_skladista(); Type: FUNCTION; Schema: public; Owner: epc
--

CREATE FUNCTION azuriranje_skladista() RETURNS trigger
    LANGUAGE plpgsql
    AS $$
DECLARE
 dubina INTEGER;
BEGIN
 dubina = pg_trigger_depth();
IF TG_OP='UPDATE' THEN
 IF dubina=3 OR dubina =2 THEN RETURN NEW; 
 ELSIF OLD.vrijeme_do='infinity'::TIMESTAMP THEN
  INSERT INTO skladista(id,vrijeme_do,lokacija,telefon,faks,oznaka) 
  VALUES(NEW.id, 'infinity'::TIMESTAMP, NEW.lokacija, NEW.telefon, NEW.faks, NEW.oznaka);
 END IF;
 RETURN NULL;
END IF;
IF TG_OP='INSERT' THEN
 UPDATE skladista SET vrijeme_do=current_timestamp WHERE 
  id=NEW.id AND vrijeme_do='infinity'::TIMESTAMP;
  RETURN NEW;
END IF;
IF TG_OP='DELETE' THEN
 UPDATE skladista SET vrijeme_do=current_timestamp WHERE
  id=OLD.id AND vrijeme_od = OLD.vrijeme_od AND 
  vrijeme_do='infinity'::TIMESTAMP;
  RETURN NULL;
END IF;
END;
$$;


ALTER FUNCTION public.azuriranje_skladista() OWNER TO epc;

--
-- Name: azuriranje_stanja(); Type: FUNCTION; Schema: public; Owner: epc
--

CREATE FUNCTION azuriranje_stanja() RETURNS trigger
    LANGUAGE plpgsql
    AS $$
DECLARE
redak_m materijali%ROWTYPE;
redak_s skladista%ROWTYPE;
dubina INTEGER;
BEGIN
 dubina = pg_trigger_depth();
 IF TG_OP='UPDATE' THEN
 IF dubina=3 OR dubina =2 THEN RETURN NEW; 
 ELSIF OLD.vrijeme_do='infinity'::TIMESTAMP THEN
  INSERT INTO stanje(skladiste,vrijeme_skladista,materijal,vrijeme_materijala,kolicina,vrijeme_do,biljeska) 
  VALUES(NEW.skladiste, NEW.vrijeme_skladista, NEW.materijal, NEW.vrijeme_materijala, NEW.kolicina, 'infinity'::TIMESTAMP, NEW.biljeska);
 END IF;
 RETURN NULL;
END IF;
IF TG_OP='INSERT' THEN
SELECT * INTO redak_m FROM materijali WHERE id=NEW.materijal AND vrijeme_do='infinity'::TIMESTAMP;
IF NOT FOUND THEN 
  RAISE EXCEPTION 'Ne postoji taj materijal.';
  RETURN NULL;
END IF;
SELECT * INTO redak_s FROM skladista  WHERE id=NEW.skladiste AND vrijeme_do='infinity'::TIMESTAMP;
IF NOT FOUND THEN 
  RAISE EXCEPTION 'Ne postoji to skladiste.';
  RETURN NULL;
END IF;

UPDATE stanje SET vrijeme_do=current_timestamp WHERE 
   materijal=NEW.materijal AND skladiste=NEW.skladiste AND vrijeme_do='infinity'::TIMESTAMP;
  NEW.vrijeme_materijala = redak_m.vrijeme_od;
  NEW.vrijeme_skladista = redak_s.vrijeme_od;
 RETURN NEW;
END IF;
IF TG_OP='DELETE' THEN
UPDATE stanje SET vrijeme_do=current_timestamp WHERE 
   materijal=OLD.materijal AND vrijeme_od = OLD.vrijeme_od AND
   skladiste=OLD.skladiste AND vrijeme_od = OLD.vrijeme_od AND 
   vrijeme_do='infinity'::TIMESTAMP;
 RETURN NULL;
END IF;
END;
$$;


ALTER FUNCTION public.azuriranje_stanja() OWNER TO epc;

--
-- Name: azuriranje_supravodica(); Type: FUNCTION; Schema: public; Owner: epc
--

CREATE FUNCTION azuriranje_supravodica() RETURNS trigger
    LANGUAGE plpgsql
    AS $$
DECLARE
redak1 dobavljaci%ROWTYPE;
redak2 materijali%ROWTYPE;
redak3 supravodici%ROWTYPE;
dubina INTEGER;
BEGIN
dubina = pg_trigger_depth();
IF TG_OP='UPDATE' THEN
 IF dubina=3 OR dubina =2 THEN RETURN NEW; 
 ELSIF OLD.vrijeme_do='infinity'::TIMESTAMP THEN
  INSERT INTO supravodici(id,dobavljac, vrijeme_dobavljaca,vrijeme_do,naziv, tip, cisti_promjer, promjer_izolator, broj_niti, cu_sc,
  "kriticna_struja3T", "kriticna_struja5T", "kriticna_struja7T", "kriticna_struja9T", promjer_niti)
  VALUES(NEW.id, NEW.dobavljac, NEW.vrijeme_dobavljaca, 'infinity'::TIMESTAMP, NEW.naziv, NEW.tip,NEW.cisti_promjer,NEW.promjer_izolator,
  NEW.broj_niti,NEW.cu_sc,NEW."kriticna_struja3T",NEW."kriticna_struja5T",NEW."kriticna_struja7T",NEW."kriticna_struja9T",NEW.promjer_niti);  
 END IF;
 RETURN NULL;
END IF;
IF TG_OP='INSERT' THEN
 SELECT * INTO redak2 FROM materijali WHERE id=NEW.id;
 IF FOUND THEN
  SELECT * INTO redak3 FROM ONLY supravodici WHERE id=NEW.id;
  IF NOT FOUND THEN
   SELECT max(id)+1 INTO NEW.id FROM materijali;
   RAISE NOTICE 'Novi id: %',NEW.id;
  END IF;
 END IF; 
 SELECT * INTO redak1 FROM dobavljaci WHERE id=NEW.dobavljac AND vrijeme_do='infinity'::TIMESTAMP;
 IF NOT FOUND THEN 
  RAISE EXCEPTION 'Ne postoji taj dobavljac.';
  RETURN NULL;
 END IF;
 UPDATE supravodici SET vrijeme_do=current_timestamp WHERE 
   id=NEW.id AND vrijeme_do='infinity'::TIMESTAMP;
  NEW.vrijeme_dobavljaca = redak1.vrijeme_od;
 RETURN NEW;
END IF;
IF TG_OP='DELETE' THEN
 UPDATE supravodici SET vrijeme_do=current_timestamp WHERE
  id=OLD.id AND vrijeme_od = OLD.vrijeme_od AND 
  vrijeme_do='infinity'::TIMESTAMP;
  RETURN NULL;
END IF;
END;
$$;


ALTER FUNCTION public.azuriranje_supravodica() OWNER TO epc;

--
-- Name: azuriranje_trake(); Type: FUNCTION; Schema: public; Owner: epc
--

CREATE FUNCTION azuriranje_trake() RETURNS trigger
    LANGUAGE plpgsql
    AS $$
DECLARE
redak1 dobavljaci%ROWTYPE;
redak2 materijali%ROWTYPE;
redak3 trake%ROWTYPE;
dubina INTEGER;
BEGIN
dubina = pg_trigger_depth();
IF TG_OP='UPDATE' THEN
 IF dubina=3 OR dubina =2 THEN RETURN NEW; 
 ELSIF OLD.vrijeme_do='infinity'::TIMESTAMP THEN
  INSERT INTO trake(id,dobavljac, vrijeme_dobavljaca,vrijeme_do,naziv, sirina,debljina,supstrat,stabilizator,krit_struja)
  VALUES(NEW.id, NEW.dobavljac, NEW.vrijeme_dobavljaca, 'infinity'::TIMESTAMP, NEW.naziv, NEW.sirina, NEW.debljina, 
  NEW.supstrat, NEW.stabilizator, NEW.krit_struja);
 END IF;
 RETURN NULL;
END IF;

IF TG_OP='INSERT' THEN
 SELECT * INTO redak2 FROM materijali WHERE id=NEW.id;
 IF FOUND THEN
  SELECT * INTO redak3 FROM ONLY trake WHERE id=NEW.id;
  IF NOT FOUND THEN
   SELECT max(id)+1 INTO NEW.id FROM materijali;
   RAISE NOTICE 'Novi id: %',NEW.id;
  END IF;
 END IF; 
 SELECT * INTO redak1 FROM dobavljaci WHERE id=NEW.dobavljac AND vrijeme_do='infinity'::TIMESTAMP;
 IF NOT FOUND THEN 
  RAISE EXCEPTION 'Ne postoji taj dobavljac.';
  RETURN NULL;
 END IF;

 UPDATE trake SET vrijeme_do=current_timestamp WHERE 
   id=NEW.id AND vrijeme_do='infinity'::TIMESTAMP;
  NEW.vrijeme_dobavljaca = redak1.vrijeme_od;
 RETURN NEW;
END IF;
IF TG_OP='DELETE' THEN
 UPDATE trake SET vrijeme_do=current_timestamp WHERE
  id=OLD.id AND vrijeme_od = OLD.vrijeme_od AND 
  vrijeme_do='infinity'::TIMESTAMP;
  RETURN NULL;
END IF;
END;
$$;


ALTER FUNCTION public.azuriranje_trake() OWNER TO epc;

SET default_tablespace = '';

SET default_with_oids = false;

--
-- Name: _pgmdd_backup_dobavljaci_2018-08-01_05:51; Type: TABLE; Schema: public; Owner: epc
--

CREATE TABLE "_pgmdd_backup_dobavljaci_2018-08-01_05:51" (
    id integer,
    vrijeme_od timestamp without time zone,
    naziv character varying(64),
    vrijeme_do timestamp without time zone,
    adresa character varying(64),
    telefon character varying(24),
    telefon2 character varying(24),
    "e-mail" character varying(128)
);


ALTER TABLE "_pgmdd_backup_dobavljaci_2018-08-01_05:51" OWNER TO epc;

--
-- Name: _pgmdd_backup_dobavljaci_2018-10-01_18:28; Type: TABLE; Schema: public; Owner: epc
--

CREATE TABLE "_pgmdd_backup_dobavljaci_2018-10-01_18:28" (
    id integer,
    vrijeme_od timestamp without time zone,
    naziv character varying(64),
    vrijeme_do timestamp without time zone,
    adresa character varying(64),
    telefon character varying(24),
    telefon2 character varying(24),
    "e-mail" character varying(128)
);


ALTER TABLE "_pgmdd_backup_dobavljaci_2018-10-01_18:28" OWNER TO epc;

--
-- Name: _pgmdd_backup_dobavljaci_2018-10-01_18:42; Type: TABLE; Schema: public; Owner: epc
--

CREATE TABLE "_pgmdd_backup_dobavljaci_2018-10-01_18:42" (
    id integer,
    vrijeme_od timestamp without time zone,
    naziv character varying(64),
    vrijeme_do timestamp without time zone,
    adresa character varying(64),
    telefon character varying(24),
    telefon2 character varying(24),
    "e-mail" character varying(128)
);


ALTER TABLE "_pgmdd_backup_dobavljaci_2018-10-01_18:42" OWNER TO epc;

--
-- Name: _pgmdd_backup_materijali_2018-08-01_05:51; Type: TABLE; Schema: public; Owner: epc
--

CREATE TABLE "_pgmdd_backup_materijali_2018-08-01_05:51" (
    id integer,
    vrijeme_od timestamp without time zone,
    dobavljac integer,
    vrijeme_dobavljaca timestamp without time zone,
    vrijeme_do timestamp without time zone,
    naziv character varying(255)
);


ALTER TABLE "_pgmdd_backup_materijali_2018-08-01_05:51" OWNER TO epc;

--
-- Name: _pgmdd_backup_materijali_2018-10-01_00:14; Type: TABLE; Schema: public; Owner: epc
--

CREATE TABLE "_pgmdd_backup_materijali_2018-10-01_00:14" (
    id integer,
    vrijeme_od timestamp without time zone,
    dobavljac integer,
    vrijeme_dobavljaca timestamp without time zone,
    vrijeme_do timestamp without time zone,
    naziv character varying(255)
);


ALTER TABLE "_pgmdd_backup_materijali_2018-10-01_00:14" OWNER TO epc;

--
-- Name: _pgmdd_backup_materijali_2018-10-01_15:43; Type: TABLE; Schema: public; Owner: epc
--

CREATE TABLE "_pgmdd_backup_materijali_2018-10-01_15:43" (
    id integer,
    vrijeme_od timestamp without time zone,
    dobavljac integer,
    vrijeme_dobavljaca timestamp without time zone,
    vrijeme_do timestamp without time zone,
    naziv character varying(255)
);


ALTER TABLE "_pgmdd_backup_materijali_2018-10-01_15:43" OWNER TO epc;

--
-- Name: _pgmdd_backup_materijali_2018-10-01_18:28; Type: TABLE; Schema: public; Owner: epc
--

CREATE TABLE "_pgmdd_backup_materijali_2018-10-01_18:28" (
    id integer,
    vrijeme_od timestamp without time zone,
    dobavljac integer,
    vrijeme_dobavljaca timestamp without time zone,
    vrijeme_do timestamp without time zone,
    naziv character varying(255)
);


ALTER TABLE "_pgmdd_backup_materijali_2018-10-01_18:28" OWNER TO epc;

--
-- Name: _pgmdd_backup_materijali_2018-10-01_18:42; Type: TABLE; Schema: public; Owner: epc
--

CREATE TABLE "_pgmdd_backup_materijali_2018-10-01_18:42" (
    id integer,
    vrijeme_od timestamp without time zone,
    dobavljac integer,
    vrijeme_dobavljaca timestamp without time zone,
    vrijeme_do timestamp without time zone,
    naziv character varying(255)
);


ALTER TABLE "_pgmdd_backup_materijali_2018-10-01_18:42" OWNER TO epc;

--
-- Name: _pgmdd_backup_shim_zavojnice_2018-10-01_18:42; Type: TABLE; Schema: public; Owner: epc
--

CREATE TABLE "_pgmdd_backup_shim_zavojnice_2018-10-01_18:42" (
    id integer,
    vrijeme_od timestamp without time zone,
    dobavljac integer,
    vrijeme_dobavljaca timestamp without time zone,
    vrijeme_do timestamp without time zone,
    naziv character varying(255),
    tip integer,
    max_struja real,
    sparivanje character varying(128),
    promjer real,
    jakost real
);


ALTER TABLE "_pgmdd_backup_shim_zavojnice_2018-10-01_18:42" OWNER TO epc;

--
-- Name: _pgmdd_backup_skladista_2018-08-01_04:42; Type: TABLE; Schema: public; Owner: epc
--

CREATE TABLE "_pgmdd_backup_skladista_2018-08-01_04:42" (
    id integer,
    vrijeme_od timestamp without time zone,
    vrijeme_do timestamp without time zone,
    lokacija character varying(64),
    telefon character varying(24),
    faks character varying(24),
    oznaka integer
);


ALTER TABLE "_pgmdd_backup_skladista_2018-08-01_04:42" OWNER TO epc;

--
-- Name: _pgmdd_backup_skladista_2018-08-01_05:51; Type: TABLE; Schema: public; Owner: epc
--

CREATE TABLE "_pgmdd_backup_skladista_2018-08-01_05:51" (
    id integer,
    vrijeme_od timestamp without time zone,
    vrijeme_do timestamp without time zone,
    lokacija character varying(64),
    telefon character varying(24),
    faks character varying(24),
    oznaka character varying(128)
);


ALTER TABLE "_pgmdd_backup_skladista_2018-08-01_05:51" OWNER TO epc;

--
-- Name: _pgmdd_backup_skladista_2018-10-01_18:42; Type: TABLE; Schema: public; Owner: epc
--

CREATE TABLE "_pgmdd_backup_skladista_2018-10-01_18:42" (
    id integer,
    vrijeme_od timestamp without time zone,
    vrijeme_do timestamp without time zone,
    lokacija character varying(64),
    telefon character varying(24),
    faks character varying(24),
    oznaka character varying(128)
);


ALTER TABLE "_pgmdd_backup_skladista_2018-10-01_18:42" OWNER TO epc;

--
-- Name: _pgmdd_backup_stanje_2018-08-01_04:42; Type: TABLE; Schema: public; Owner: epc
--

CREATE TABLE "_pgmdd_backup_stanje_2018-08-01_04:42" (
    skladiste integer,
    vrijeme_skladista timestamp without time zone,
    materijal integer,
    vrijeme_materijala timestamp without time zone,
    vrijeme_od timestamp without time zone,
    kolicina integer,
    vrijeme_do timestamp without time zone
);


ALTER TABLE "_pgmdd_backup_stanje_2018-08-01_04:42" OWNER TO epc;

--
-- Name: _pgmdd_backup_stanje_2018-08-01_05:51; Type: TABLE; Schema: public; Owner: epc
--

CREATE TABLE "_pgmdd_backup_stanje_2018-08-01_05:51" (
    skladiste integer,
    vrijeme_skladista timestamp without time zone,
    materijal integer,
    vrijeme_materijala timestamp without time zone,
    vrijeme_od timestamp without time zone,
    kolicina integer,
    vrijeme_do timestamp without time zone,
    skladista_oznaka character varying(128)
);


ALTER TABLE "_pgmdd_backup_stanje_2018-08-01_05:51" OWNER TO epc;

--
-- Name: _pgmdd_backup_stanje_2018-10-01_00:14; Type: TABLE; Schema: public; Owner: epc
--

CREATE TABLE "_pgmdd_backup_stanje_2018-10-01_00:14" (
    skladiste integer,
    vrijeme_skladista timestamp without time zone,
    materijal integer,
    vrijeme_materijala timestamp without time zone,
    vrijeme_od timestamp without time zone,
    kolicina integer,
    vrijeme_do timestamp without time zone,
    skladista_oznaka character varying(128)
);


ALTER TABLE "_pgmdd_backup_stanje_2018-10-01_00:14" OWNER TO epc;

--
-- Name: _pgmdd_backup_stanje_2018-10-01_00:32; Type: TABLE; Schema: public; Owner: epc
--

CREATE TABLE "_pgmdd_backup_stanje_2018-10-01_00:32" (
    skladiste integer,
    vrijeme_skladista timestamp without time zone,
    materijal integer,
    vrijeme_materijala timestamp without time zone,
    vrijeme_od timestamp without time zone,
    kolicina integer,
    vrijeme_do timestamp without time zone,
    biljeska character varying(1024)
);


ALTER TABLE "_pgmdd_backup_stanje_2018-10-01_00:32" OWNER TO epc;

--
-- Name: _pgmdd_backup_stanje_2018-10-01_00:35; Type: TABLE; Schema: public; Owner: epc
--

CREATE TABLE "_pgmdd_backup_stanje_2018-10-01_00:35" (
    skladiste integer,
    vrijeme_skladista timestamp without time zone,
    materijal integer,
    vrijeme_materijala timestamp without time zone,
    vrijeme_od timestamp without time zone,
    kolicina integer,
    vrijeme_do timestamp without time zone,
    biljeska character varying(1024)
);


ALTER TABLE "_pgmdd_backup_stanje_2018-10-01_00:35" OWNER TO epc;

--
-- Name: _pgmdd_backup_stanje_2018-10-01_01:58; Type: TABLE; Schema: public; Owner: epc
--

CREATE TABLE "_pgmdd_backup_stanje_2018-10-01_01:58" (
    skladiste integer,
    vrijeme_skladista timestamp without time zone,
    materijal integer,
    vrijeme_materijala timestamp without time zone,
    vrijeme_od timestamp without time zone,
    kolicina integer,
    vrijeme_do timestamp without time zone,
    biljeska character varying(1024)
);


ALTER TABLE "_pgmdd_backup_stanje_2018-10-01_01:58" OWNER TO epc;

--
-- Name: _pgmdd_backup_stanje_2018-10-01_15:43; Type: TABLE; Schema: public; Owner: epc
--

CREATE TABLE "_pgmdd_backup_stanje_2018-10-01_15:43" (
    skladiste integer,
    vrijeme_skladista timestamp without time zone,
    materijal integer,
    vrijeme_materijala timestamp without time zone,
    vrijeme_od timestamp without time zone,
    kolicina integer,
    vrijeme_do timestamp without time zone,
    biljeska character varying(1024)
);


ALTER TABLE "_pgmdd_backup_stanje_2018-10-01_15:43" OWNER TO epc;

--
-- Name: _pgmdd_backup_stanje_2018-10-01_18:28; Type: TABLE; Schema: public; Owner: epc
--

CREATE TABLE "_pgmdd_backup_stanje_2018-10-01_18:28" (
    skladiste integer,
    vrijeme_skladista timestamp without time zone,
    materijal integer,
    vrijeme_materijala timestamp without time zone,
    vrijeme_od timestamp without time zone,
    kolicina integer,
    vrijeme_do timestamp without time zone,
    biljeska character varying(1024)
);


ALTER TABLE "_pgmdd_backup_stanje_2018-10-01_18:28" OWNER TO epc;

--
-- Name: _pgmdd_backup_stanje_2018-10-01_18:42; Type: TABLE; Schema: public; Owner: epc
--

CREATE TABLE "_pgmdd_backup_stanje_2018-10-01_18:42" (
    skladiste integer,
    vrijeme_skladista timestamp without time zone,
    materijal integer,
    vrijeme_materijala timestamp without time zone,
    vrijeme_od timestamp without time zone,
    kolicina integer,
    vrijeme_do timestamp without time zone,
    biljeska character varying(1024)
);


ALTER TABLE "_pgmdd_backup_stanje_2018-10-01_18:42" OWNER TO epc;

--
-- Name: _pgmdd_backup_supravodici_2018-10-01_00:35; Type: TABLE; Schema: public; Owner: epc
--

CREATE TABLE "_pgmdd_backup_supravodici_2018-10-01_00:35" (
    id integer,
    vrijeme_od timestamp without time zone,
    dobavljac integer,
    vrijeme_dobavljaca timestamp without time zone,
    vrijeme_do timestamp without time zone,
    naziv character varying(255),
    tip integer,
    cisti_promjer real,
    promjer_izolator real,
    broj_niti integer,
    cu_sc real,
    "kriticnaStruja3T" real,
    "kriticna_struja5T" real,
    "kriticna_struja7T" real,
    "kriticna_struja9T" real,
    promjer_niti real
);


ALTER TABLE "_pgmdd_backup_supravodici_2018-10-01_00:35" OWNER TO epc;

--
-- Name: _pgmdd_backup_supravodici_2018-10-01_01:58; Type: TABLE; Schema: public; Owner: epc
--

CREATE TABLE "_pgmdd_backup_supravodici_2018-10-01_01:58" (
    id integer,
    vrijeme_od timestamp without time zone,
    dobavljac integer,
    vrijeme_dobavljaca timestamp without time zone,
    vrijeme_do timestamp without time zone,
    naziv character varying(255),
    tip character varying(24),
    cisti_promjer real,
    promjer_izolator real,
    broj_niti integer,
    cu_sc real,
    "kriticnaStruja3T" real,
    "kriticna_struja5T" real,
    "kriticna_struja7T" real,
    "kriticna_struja9T" real,
    promjer_niti real
);


ALTER TABLE "_pgmdd_backup_supravodici_2018-10-01_01:58" OWNER TO epc;

--
-- Name: _pgmdd_backup_supravodici_2018-10-01_15:43; Type: TABLE; Schema: public; Owner: epc
--

CREATE TABLE "_pgmdd_backup_supravodici_2018-10-01_15:43" (
    id integer,
    vrijeme_od timestamp without time zone,
    dobavljac integer,
    vrijeme_dobavljaca timestamp without time zone,
    vrijeme_do timestamp without time zone,
    naziv character varying(255),
    tip character varying(24),
    cisti_promjer real,
    promjer_izolator real,
    broj_niti integer,
    cu_sc real,
    "kriticnaStruja3T" real,
    "kriticna_struja5T" real,
    "kriticna_struja7T" real,
    "kriticna_struja9T" real,
    promjer_niti real
);


ALTER TABLE "_pgmdd_backup_supravodici_2018-10-01_15:43" OWNER TO epc;

--
-- Name: _pgmdd_backup_supravodici_2018-10-01_18:28; Type: TABLE; Schema: public; Owner: epc
--

CREATE TABLE "_pgmdd_backup_supravodici_2018-10-01_18:28" (
    id integer,
    vrijeme_od timestamp without time zone,
    dobavljac integer,
    vrijeme_dobavljaca timestamp without time zone,
    vrijeme_do timestamp without time zone,
    naziv character varying(255),
    tip character varying(24),
    cisti_promjer real,
    promjer_izolator real,
    broj_niti integer,
    cu_sc real,
    "kriticna_struja3T" real,
    "kriticna_struja5T" real,
    "kriticna_struja7T" real,
    "kriticna_struja9T" real,
    promjer_niti real
);


ALTER TABLE "_pgmdd_backup_supravodici_2018-10-01_18:28" OWNER TO epc;

--
-- Name: _pgmdd_backup_supravodici_2018-10-01_18:42; Type: TABLE; Schema: public; Owner: epc
--

CREATE TABLE "_pgmdd_backup_supravodici_2018-10-01_18:42" (
    id integer,
    vrijeme_od timestamp without time zone,
    dobavljac integer,
    vrijeme_dobavljaca timestamp without time zone,
    vrijeme_do timestamp without time zone,
    naziv character varying(255),
    tip character varying(24),
    cisti_promjer real,
    promjer_izolator real,
    broj_niti integer,
    cu_sc real,
    "kriticna_struja3T" real,
    "kriticna_struja5T" real,
    "kriticna_struja7T" real,
    "kriticna_struja9T" real,
    promjer_niti real
);


ALTER TABLE "_pgmdd_backup_supravodici_2018-10-01_18:42" OWNER TO epc;

--
-- Name: dobavljaci; Type: TABLE; Schema: public; Owner: epc
--

CREATE TABLE dobavljaci (
    id integer NOT NULL,
    vrijeme_od timestamp without time zone DEFAULT now() NOT NULL,
    naziv character varying(64) NOT NULL,
    vrijeme_do timestamp without time zone DEFAULT 'infinity'::timestamp without time zone NOT NULL,
    adresa character varying(64),
    telefon character varying(24),
    telefon2 character varying(24),
    "e-mail" character varying(128)
);


ALTER TABLE dobavljaci OWNER TO epc;

--
-- Name: dobavljaci_id_seq; Type: SEQUENCE; Schema: public; Owner: epc
--

CREATE SEQUENCE dobavljaci_id_seq
    AS integer
    START WITH 1
    INCREMENT BY 1
    NO MINVALUE
    NO MAXVALUE
    CACHE 1;


ALTER TABLE dobavljaci_id_seq OWNER TO epc;

--
-- Name: dobavljaci_id_seq; Type: SEQUENCE OWNED BY; Schema: public; Owner: epc
--

ALTER SEQUENCE dobavljaci_id_seq OWNED BY dobavljaci.id;


--
-- Name: materijali; Type: TABLE; Schema: public; Owner: epc
--

CREATE TABLE materijali (
    id integer NOT NULL,
    vrijeme_od timestamp without time zone DEFAULT now() NOT NULL,
    dobavljac integer NOT NULL,
    vrijeme_dobavljaca timestamp without time zone DEFAULT now() NOT NULL,
    vrijeme_do timestamp without time zone DEFAULT 'infinity'::timestamp without time zone NOT NULL,
    naziv character varying(255)
);


ALTER TABLE materijali OWNER TO epc;

--
-- Name: materijali_id_seq; Type: SEQUENCE; Schema: public; Owner: epc
--

CREATE SEQUENCE materijali_id_seq
    AS integer
    START WITH 1
    INCREMENT BY 1
    NO MINVALUE
    NO MAXVALUE
    CACHE 1;


ALTER TABLE materijali_id_seq OWNER TO epc;

--
-- Name: materijali_id_seq; Type: SEQUENCE OWNED BY; Schema: public; Owner: epc
--

ALTER SEQUENCE materijali_id_seq OWNED BY materijali.id;


--
-- Name: shim_zavojnice; Type: TABLE; Schema: public; Owner: epc
--

CREATE TABLE shim_zavojnice (
    tip character varying(16) NOT NULL,
    max_struja real,
    sparivanje character varying(128),
    promjer real,
    jakost real
)
INHERITS (materijali);


ALTER TABLE shim_zavojnice OWNER TO epc;

--
-- Name: skladista; Type: TABLE; Schema: public; Owner: epc
--

CREATE TABLE skladista (
    id integer NOT NULL,
    vrijeme_od timestamp without time zone DEFAULT now() NOT NULL,
    vrijeme_do timestamp without time zone DEFAULT 'infinity'::timestamp without time zone NOT NULL,
    lokacija character varying(64),
    telefon character varying(24),
    faks character varying(24),
    oznaka character varying(128) NOT NULL
);


ALTER TABLE skladista OWNER TO epc;

--
-- Name: skladista_id_seq; Type: SEQUENCE; Schema: public; Owner: epc
--

CREATE SEQUENCE skladista_id_seq
    AS integer
    START WITH 1
    INCREMENT BY 1
    NO MINVALUE
    NO MAXVALUE
    CACHE 1;


ALTER TABLE skladista_id_seq OWNER TO epc;

--
-- Name: skladista_id_seq; Type: SEQUENCE OWNED BY; Schema: public; Owner: epc
--

ALTER SEQUENCE skladista_id_seq OWNED BY skladista.id;


--
-- Name: stanje; Type: TABLE; Schema: public; Owner: epc
--

CREATE TABLE stanje (
    skladiste integer NOT NULL,
    vrijeme_skladista timestamp without time zone NOT NULL,
    materijal integer NOT NULL,
    vrijeme_materijala timestamp without time zone NOT NULL,
    vrijeme_od timestamp without time zone DEFAULT now() NOT NULL,
    kolicina integer DEFAULT 0 NOT NULL,
    vrijeme_do timestamp without time zone DEFAULT 'infinity'::timestamp without time zone NOT NULL,
    biljeska character varying(1024)
);


ALTER TABLE stanje OWNER TO epc;

--
-- Name: supravodici; Type: TABLE; Schema: public; Owner: epc
--

CREATE TABLE supravodici (
    tip character varying(24) NOT NULL,
    cisti_promjer real,
    promjer_izolator real,
    broj_niti integer,
    cu_sc real,
    "kriticna_struja3T" real,
    "kriticna_struja5T" real,
    "kriticna_struja7T" real,
    "kriticna_struja9T" real,
    promjer_niti real
)
INHERITS (materijali);


ALTER TABLE supravodici OWNER TO epc;

--
-- Name: trake; Type: TABLE; Schema: public; Owner: epc
--

CREATE TABLE trake (
    sirina real,
    debljina real,
    supstrat real,
    stabilizator real,
    krit_struja real
)
INHERITS (materijali);


ALTER TABLE trake OWNER TO epc;

--
-- Name: dobavljaci id; Type: DEFAULT; Schema: public; Owner: epc
--

ALTER TABLE ONLY dobavljaci ALTER COLUMN id SET DEFAULT nextval('dobavljaci_id_seq'::regclass);


--
-- Name: materijali id; Type: DEFAULT; Schema: public; Owner: epc
--

ALTER TABLE ONLY materijali ALTER COLUMN id SET DEFAULT nextval('materijali_id_seq'::regclass);


--
-- Name: shim_zavojnice id; Type: DEFAULT; Schema: public; Owner: epc
--

ALTER TABLE ONLY shim_zavojnice ALTER COLUMN id SET DEFAULT nextval('materijali_id_seq'::regclass);


--
-- Name: shim_zavojnice vrijeme_od; Type: DEFAULT; Schema: public; Owner: epc
--

ALTER TABLE ONLY shim_zavojnice ALTER COLUMN vrijeme_od SET DEFAULT now();


--
-- Name: shim_zavojnice vrijeme_dobavljaca; Type: DEFAULT; Schema: public; Owner: epc
--

ALTER TABLE ONLY shim_zavojnice ALTER COLUMN vrijeme_dobavljaca SET DEFAULT now();


--
-- Name: shim_zavojnice vrijeme_do; Type: DEFAULT; Schema: public; Owner: epc
--

ALTER TABLE ONLY shim_zavojnice ALTER COLUMN vrijeme_do SET DEFAULT 'infinity'::timestamp without time zone;


--
-- Name: skladista id; Type: DEFAULT; Schema: public; Owner: epc
--

ALTER TABLE ONLY skladista ALTER COLUMN id SET DEFAULT nextval('skladista_id_seq'::regclass);


--
-- Name: supravodici id; Type: DEFAULT; Schema: public; Owner: epc
--

ALTER TABLE ONLY supravodici ALTER COLUMN id SET DEFAULT nextval('materijali_id_seq'::regclass);


--
-- Name: supravodici vrijeme_od; Type: DEFAULT; Schema: public; Owner: epc
--

ALTER TABLE ONLY supravodici ALTER COLUMN vrijeme_od SET DEFAULT now();


--
-- Name: supravodici vrijeme_dobavljaca; Type: DEFAULT; Schema: public; Owner: epc
--

ALTER TABLE ONLY supravodici ALTER COLUMN vrijeme_dobavljaca SET DEFAULT now();


--
-- Name: supravodici vrijeme_do; Type: DEFAULT; Schema: public; Owner: epc
--

ALTER TABLE ONLY supravodici ALTER COLUMN vrijeme_do SET DEFAULT 'infinity'::timestamp without time zone;


--
-- Name: trake id; Type: DEFAULT; Schema: public; Owner: epc
--

ALTER TABLE ONLY trake ALTER COLUMN id SET DEFAULT nextval('materijali_id_seq'::regclass);


--
-- Name: trake vrijeme_od; Type: DEFAULT; Schema: public; Owner: epc
--

ALTER TABLE ONLY trake ALTER COLUMN vrijeme_od SET DEFAULT now();


--
-- Name: trake vrijeme_dobavljaca; Type: DEFAULT; Schema: public; Owner: epc
--

ALTER TABLE ONLY trake ALTER COLUMN vrijeme_dobavljaca SET DEFAULT now();


--
-- Name: trake vrijeme_do; Type: DEFAULT; Schema: public; Owner: epc
--

ALTER TABLE ONLY trake ALTER COLUMN vrijeme_do SET DEFAULT 'infinity'::timestamp without time zone;



--
-- Data for Name: dobavljaci; Type: TABLE DATA; Schema: public; Owner: epc
--

COPY dobavljaci (id, vrijeme_od, naziv, vrijeme_do, adresa, telefon, telefon2, "e-mail") FROM stdin;
1	2018-01-08 04:19:05.176647	D1	2018-01-08 04:19:05.176647	\N	\N	\N	\N
1	2018-01-10 15:22:07.552919	Supercon Inc.	infinity	\N	\N	\N	\N
1	2018-01-08 04:19:37.661234	D1	2018-01-10 15:22:07.552919	\N	\N	\N	\N
1	2018-01-10 15:18:13.241298	Supercon Inc.	2018-01-10 15:22:07.552919	\N	\N	\N	\N
2	2018-02-18 21:24:10.28095	Fujikura	infinity	Leatherhead Road,Chessington,Surrey,KT9 2NY, UK	+44 (0) 20 8240 2000	\N	sales@fujikura.co.uk
\.


--
-- Data for Name: materijali; Type: TABLE DATA; Schema: public; Owner: epc
--

COPY materijali (id, vrijeme_od, dobavljac, vrijeme_dobavljaca, vrijeme_do, naziv) FROM stdin;
2	2018-01-08 23:42:08.040142	1	2018-01-08 04:19:37.661234	2018-01-08 23:49:52.066138	M1
2	2018-01-08 23:49:52.066138	1	2018-01-08 04:19:37.661234	2018-01-08 23:50:00.620953	M1
2	2018-01-08 23:50:00.620953	1	2018-01-08 04:19:37.661234	2018-01-08 23:50:00.620953	M1
3	2018-01-08 04:19:37.661234	1	2018-01-08 04:19:37.661234	infinity	M1
1	2018-01-08 05:38:06.173538	1	2018-01-08 04:19:37.661234	2018-01-08 05:38:14.892007	M1
1	2018-01-08 05:38:14.892007	1	2018-01-08 04:19:37.661234	2018-01-08 05:39:22.391835	M1
1	2018-01-08 05:39:22.391835	1	2018-01-08 04:19:37.661234	2018-01-08 05:39:27.480574	M1
1	2018-01-08 05:39:27.480574	1	2018-01-08 04:19:37.661234	2018-01-08 05:39:27.480574	M1
1	2018-01-08 05:43:32.613408	1	2018-01-08 04:19:37.661234	2018-01-08 05:43:37.816698	M1
1	2018-01-08 05:43:37.816698	1	2018-01-08 04:19:37.661234	2018-01-08 05:44:05.228213	M1
1	2018-01-08 05:50:14.104324	1	2018-01-08 04:19:37.661234	2018-01-08 05:50:14.104324	M1
1	2018-01-08 23:39:16.750085	1	2018-01-08 04:19:37.661234	2018-01-08 23:39:26.13482	M1
1	2018-01-08 23:39:26.13482	1	2018-01-08 04:19:37.661234	2018-01-08 23:41:19.585944	M1
1	2018-01-08 23:41:19.585944	1	2018-01-08 04:19:37.661234	2018-01-08 23:41:24.594381	M1
1	2018-01-08 23:41:24.594381	1	2018-01-08 04:19:37.661234	infinity	M1
2	2018-01-08 23:42:01.167462	1	2018-01-08 04:19:37.661234	2018-01-08 23:42:08.040142	M1
4	2018-01-08 23:42:21.000374	1	2018-01-08 04:19:37.661234	infinity	M1
\.


--
-- Data for Name: shim_zavojnice; Type: TABLE DATA; Schema: public; Owner: epc
--

COPY shim_zavojnice (id, vrijeme_od, dobavljac, vrijeme_dobavljaca, vrijeme_do, naziv, tip, max_struja, sparivanje, promjer, jakost) FROM stdin;
14	2018-01-10 17:43:44.69614	1	2018-01-10 15:22:07.552919	infinity	SH-Z1	Z1	25	Preko glavne zavojnice	10	10.8000002
15	2018-01-10 17:44:21.360294	1	2018-01-10 15:22:07.552919	infinity	SH-Z1	Z2	25	Preko glavne zavojnice	10	4.73999977
\.


--
-- Data for Name: skladista; Type: TABLE DATA; Schema: public; Owner: epc
--

COPY skladista (id, vrijeme_od, vrijeme_do, lokacija, telefon, faks, oznaka) FROM stdin;
1	2018-01-08 04:12:56.027086	infinity	\N	\N	\N	S1
1	2018-01-08 04:02:30.817858	2018-01-08 04:02:37.849104	\N	\N	\N	S1
1	2018-01-08 04:02:37.849104	2018-01-08 04:04:59.241159	\N	\N	\N	S1
1	2018-01-08 04:05:52.83625	2018-01-08 04:05:58.014813	\N	\N	\N	S1
1	2018-01-08 04:09:40.086676	2018-01-08 04:09:40.086676	\N	\N	\N	S1
1	2018-01-08 04:11:34.78791	2018-01-08 04:11:34.78791	\N	\N	\N	S1
1	2018-01-08 04:12:29.72161	2018-01-08 04:12:35.660325	\N	\N	\N	S1
1	2018-01-08 04:12:35.660325	2018-01-08 04:12:35.660325	\N	\N	\N	S1
1	2018-01-08 04:12:47.197856	2018-01-08 04:12:47.197856	\N	\N	\N	S1
\.


--
-- Data for Name: stanje; Type: TABLE DATA; Schema: public; Owner: epc
--

COPY stanje (skladiste, vrijeme_skladista, materijal, vrijeme_materijala, vrijeme_od, kolicina, vrijeme_do, biljeska) FROM stdin;
1	2018-01-08 04:12:56.027086	5	2018-01-10 14:57:17.900401	2018-02-18 09:02:59.226027	5	infinity	\N
1	2018-01-08 04:12:56.027086	1	2018-01-08 23:41:24.594381	2018-02-18 09:02:38.04763	10	2018-02-18 09:03:08.259024	\N
1	2018-01-08 04:12:56.027086	1	2018-01-08 23:41:24.594381	2018-02-18 09:03:08.259024	2	infinity	\N
1	2018-01-08 04:12:56.027086	3	2018-01-08 04:19:37.661234	2018-02-18 09:02:46.774167	10	2018-02-18 09:04:01.432519	\N
1	2018-01-08 04:12:56.027086	3	2018-01-08 04:19:37.661234	2018-02-18 09:04:01.432519	2	infinity	\N
\.


--
-- Data for Name: supravodici; Type: TABLE DATA; Schema: public; Owner: epc
--

COPY supravodici (id, vrijeme_od, dobavljac, vrijeme_dobavljaca, vrijeme_do, naziv, tip, cisti_promjer, promjer_izolator, broj_niti, cu_sc, "kriticna_struja3T", "kriticna_struja5T", "kriticna_struja7T", "kriticna_struja9T", promjer_niti) FROM stdin;
5	2018-01-10 00:05:13.989154	1	2018-01-08 04:19:37.661234	2018-01-10 00:05:23.658221	S1	T1	\N	\N	\N	\N	\N	\N	\N	\N	\N
5	2018-01-10 00:05:23.658221	1	2018-01-08 04:19:37.661234	2018-01-10 00:05:23.658221	S1	T2	\N	\N	\N	\N	\N	\N	\N	\N	\N
5	2018-01-10 00:24:09.83506	1	2018-01-08 04:19:37.661234	2018-01-10 00:35:29.525664	S1	T2	\N	\N	\N	\N	\N	\N	\N	\N	\N
7	2018-01-10 00:34:43.620025	1	2018-01-08 04:19:37.661234	2018-01-10 00:35:51.141726	S1	T2	\N	\N	\N	\N	\N	\N	\N	\N	\N
7	2018-01-10 00:35:51.141726	1	2018-01-08 04:19:37.661234	2018-01-10 00:42:40.225811	S1	T2	\N	\N	\N	\N	\N	\N	\N	\N	\N
7	2018-01-10 00:42:40.225811	1	2018-01-08 04:19:37.661234	2018-01-10 00:42:40.225811	S1	T2	\N	\N	\N	\N	\N	\N	\N	\N	\N
5	2018-01-10 00:35:29.525664	1	2018-01-08 04:19:37.661234	2018-01-10 14:56:47.651286	S1	T2	\N	\N	\N	\N	\N	\N	\N	\N	\N
5	2018-01-10 14:56:47.651286	1	2018-01-08 04:19:37.661234	2018-01-10 14:57:17.900401	Multifilament Wire	56S53	0.300000012	0.330000013	56	0.899999976	125	100	55	20	30
5	2018-01-10 14:57:17.900401	1	2018-01-08 04:19:37.661234	infinity	Multifilament Wire	56S53	0.300000012	0.330000013	56	0.899999976	125	100	55	20	30
8	2018-01-10 00:36:08.447021	1	2018-01-08 04:19:37.661234	2018-01-10 15:02:13.791605	S1	T2	\N	\N	\N	\N	\N	\N	\N	\N	\N
8	2018-01-10 15:02:13.791605	1	2018-01-08 04:19:37.661234	infinity	Multifilament Wire	54S43	0.300000012	0.330000013	54	1.29999995	100	80	45	16	25
10	2018-01-10 00:42:06.049901	1	2018-01-08 04:19:37.661234	2018-01-10 15:02:13.809029	S1	T2	\N	\N	\N	\N	\N	\N	\N	\N	\N
10	2018-01-10 15:02:13.809029	1	2018-01-08 04:19:37.661234	infinity	Multifilament Wire	54S33	0.400000006	0.430000007	54	2	150	110	70	23	31
6	2018-01-10 15:07:03.767644	1	2018-01-08 04:19:37.661234	infinity	Multifilament Wire	MR24	0.699999988	0.75	24	7	160	120	75	25	50
7	2018-01-10 15:07:03.781099	1	2018-01-08 04:19:37.661234	infinity	Multifilament Wire	MR16	1.25	1.29999995	16	11	345	235	160	50	90
11	2018-01-10 15:12:56.372402	1	2018-01-08 04:19:37.661234	infinity	Monofilament Wire	T48B-M	0.300000012	0.330000013	1	1.5	130	90	60	20	185
12	2018-01-10 15:15:33.903181	1	2018-01-08 04:19:37.661234	infinity	Monofilament Wire	T48B-G	0.400000006	0.430000007	1	3	110	80	50	18	200
13	2018-01-10 15:15:33.914603	1	2018-01-08 04:19:37.661234	infinity	Monofilament Wire	T48B-G	0.699999988	0.753000021	1	3	250	180	110	45	350
\.


--
-- Data for Name: trake; Type: TABLE DATA; Schema: public; Owner: epc
--

COPY trake (id, vrijeme_od, dobavljac, vrijeme_dobavljaca, vrijeme_do, naziv, sirina, debljina, supstrat, stabilizator, krit_struja) FROM stdin;
17	2018-02-18 21:38:59.641284	2	2018-02-18 21:38:59.641284	infinity	FYSC-SC05	5	0.159999996	75	75	250
18	2018-02-18 21:39:41.153549	2	2018-02-18 21:39:41.153549	infinity	FYSC-SC10	10	0.159999996	75	75	500
19	2018-02-18 21:40:23.537238	2	2018-02-18 21:40:23.537238	infinity	FYSC-S05	5	0.0799999982	75	0	250
20	2018-02-18 21:40:48.152601	2	2018-02-18 21:40:48.152601	infinity	FYSC-S10	10	0.0799999982	75	0	500
\.


--
-- Name: dobavljaci_id_seq; Type: SEQUENCE SET; Schema: public; Owner: epc
--

SELECT pg_catalog.setval('dobavljaci_id_seq', 1, false);


--
-- Name: materijali_id_seq; Type: SEQUENCE SET; Schema: public; Owner: epc
--

SELECT pg_catalog.setval('materijali_id_seq', 20, true);


--
-- Name: skladista_id_seq; Type: SEQUENCE SET; Schema: public; Owner: epc
--

SELECT pg_catalog.setval('skladista_id_seq', 1, true);


--
-- Name: dobavljaci dobavljaci_pkey; Type: CONSTRAINT; Schema: public; Owner: epc
--

ALTER TABLE ONLY dobavljaci
    ADD CONSTRAINT dobavljaci_pkey PRIMARY KEY (id, vrijeme_od);


--
-- Name: materijali materijali_pkey; Type: CONSTRAINT; Schema: public; Owner: epc
--

ALTER TABLE ONLY materijali
    ADD CONSTRAINT materijali_pkey PRIMARY KEY (id, vrijeme_od);


--
-- Name: skladista skladista_pkey; Type: CONSTRAINT; Schema: public; Owner: epc
--

ALTER TABLE ONLY skladista
    ADD CONSTRAINT skladista_pkey PRIMARY KEY (id, vrijeme_od);


--
-- Name: stanje stanje_pkey; Type: CONSTRAINT; Schema: public; Owner: epc
--

ALTER TABLE ONLY stanje
    ADD CONSTRAINT stanje_pkey PRIMARY KEY (skladiste, vrijeme_od, materijal, vrijeme_skladista, vrijeme_materijala);


--
-- Name: dobavljaci dobavljaci_okidac; Type: TRIGGER; Schema: public; Owner: epc
--

CREATE TRIGGER dobavljaci_okidac BEFORE INSERT OR DELETE OR UPDATE ON dobavljaci FOR EACH ROW EXECUTE PROCEDURE azuriranje_dobavljaca();


--
-- Name: materijali materijali_okidac; Type: TRIGGER; Schema: public; Owner: epc
--

CREATE TRIGGER materijali_okidac BEFORE INSERT OR DELETE OR UPDATE ON materijali FOR EACH ROW EXECUTE PROCEDURE azuriranje_materijala();


--
-- Name: shim_zavojnice shim_zavojnice_okidac; Type: TRIGGER; Schema: public; Owner: epc
--

CREATE TRIGGER shim_zavojnice_okidac BEFORE INSERT OR DELETE OR UPDATE ON shim_zavojnice FOR EACH ROW EXECUTE PROCEDURE azuriranje_shim_zavojnice();


--
-- Name: skladista skladista_okidac; Type: TRIGGER; Schema: public; Owner: epc
--

CREATE TRIGGER skladista_okidac BEFORE INSERT OR DELETE OR UPDATE ON skladista FOR EACH ROW EXECUTE PROCEDURE azuriranje_skladista();


--
-- Name: stanje stanje_okidac; Type: TRIGGER; Schema: public; Owner: epc
--

CREATE TRIGGER stanje_okidac BEFORE INSERT OR DELETE OR UPDATE ON stanje FOR EACH ROW EXECUTE PROCEDURE azuriranje_stanja();


--
-- Name: supravodici supravodici_okidac; Type: TRIGGER; Schema: public; Owner: epc
--

CREATE TRIGGER supravodici_okidac BEFORE INSERT OR DELETE OR UPDATE ON supravodici FOR EACH ROW EXECUTE PROCEDURE azuriranje_supravodica();


--
-- Name: trake trake_okidac; Type: TRIGGER; Schema: public; Owner: epc
--

CREATE TRIGGER trake_okidac BEFORE INSERT OR DELETE OR UPDATE ON trake FOR EACH ROW EXECUTE PROCEDURE azuriranje_trake();


--
-- Name: materijali Ref_materijali_to_dobavljaci; Type: FK CONSTRAINT; Schema: public; Owner: epc
--

ALTER TABLE ONLY materijali
    ADD CONSTRAINT "Ref_materijali_to_dobavljaci" FOREIGN KEY (dobavljac, vrijeme_dobavljaca) REFERENCES dobavljaci(id, vrijeme_od);


--
-- Name: stanje Ref_stanje_to_skladista; Type: FK CONSTRAINT; Schema: public; Owner: epc
--

ALTER TABLE ONLY stanje
    ADD CONSTRAINT "Ref_stanje_to_skladista" FOREIGN KEY (skladiste, vrijeme_skladista) REFERENCES skladista(id, vrijeme_od);


--
-- PostgreSQL database dump complete
--

