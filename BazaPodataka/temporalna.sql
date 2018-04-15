--
-- PostgreSQL database dump
--

-- Dumped from database version 9.5.11
-- Dumped by pg_dump version 9.5.11

SET statement_timeout = 0;
SET lock_timeout = 0;
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
-- Name: azuriranje_stitova(); Type: FUNCTION; Schema: public; Owner: epc
--

CREATE FUNCTION azuriranje_stitova() RETURNS trigger
    LANGUAGE plpgsql
    AS $$
DECLARE
redak1 dobavljaci%ROWTYPE;
redak2 materijali%ROWTYPE;
redak3 stitovi%ROWTYPE;
dubina INTEGER;
BEGIN
dubina = pg_trigger_depth();
IF TG_OP='UPDATE' THEN
 IF dubina=3 OR dubina =2 THEN RETURN NEW; 
 ELSIF OLD.vrijeme_do='infinity'::TIMESTAMP THEN
  INSERT INTO stitovi(id,dobavljac, vrijeme_dobavljaca,vrijeme_do,naziv, materijal,gustoca,debljina_zida,
  gustoca_mag_toka,faktor_zastite,efikasnost_zastite,krit_temp_zero,krit_temp_srednje,unut_promjer,duljina)
  VALUES(NEW.id, NEW.dobavljac, NEW.vrijeme_dobavljaca, 'infinity'::TIMESTAMP, NEW.naziv, NEW.materijal,NEW.gustoca,
  NEW.debljina_zida,NEW.gustoca_mag_toka,NEW.faktor_zastite,NEW.efikasnost_zastite,NEW.krit_temp_zero,
  NEW.krit_temp_srednje,NEW.unut_promjer,NEW.duljina);  
 END IF;
 RETURN NULL;
END IF;
IF TG_OP='INSERT' THEN
 SELECT * INTO redak2 FROM materijali WHERE id=NEW.id;
 IF FOUND THEN
  SELECT * INTO redak3 FROM ONLY stitovi WHERE id=NEW.id;
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
 UPDATE stitovi SET vrijeme_do=current_timestamp WHERE 
   id=NEW.id AND vrijeme_do='infinity'::TIMESTAMP;
  NEW.vrijeme_dobavljaca = redak1.vrijeme_od;
 RETURN NEW;
END IF;
IF TG_OP='DELETE' THEN
 UPDATE stitovi SET vrijeme_do=current_timestamp WHERE
  id=OLD.id AND vrijeme_od = OLD.vrijeme_od AND 
  vrijeme_do='infinity'::TIMESTAMP;
  RETURN NULL;
END IF;
END;
$$;


ALTER FUNCTION public.azuriranje_stitova() OWNER TO epc;

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
-- Name: dimenzije; Type: TABLE; Schema: public; Owner: epc
--

CREATE TABLE dimenzije (
    id integer NOT NULL,
    tablica_podtip character varying(48),
    "1" character varying(16),
    "2" character varying(16),
    "3" character varying(16),
    "4" character varying(16),
    "5" character varying(16),
    "6" character varying(16),
    "7" character varying(16),
    "8" character varying(16),
    "9" character varying(16),
    "10" character varying(16)
);


ALTER TABLE dimenzije OWNER TO epc;

--
-- Name: dimenzije_id_seq; Type: SEQUENCE; Schema: public; Owner: epc
--

CREATE SEQUENCE dimenzije_id_seq
    START WITH 1
    INCREMENT BY 1
    NO MINVALUE
    NO MAXVALUE
    CACHE 1;


ALTER TABLE dimenzije_id_seq OWNER TO epc;

--
-- Name: dimenzije_id_seq; Type: SEQUENCE OWNED BY; Schema: public; Owner: epc
--

ALTER SEQUENCE dimenzije_id_seq OWNED BY dimenzije.id;


--
-- Name: dobavljaci_seq; Type: SEQUENCE; Schema: public; Owner: epc
--

CREATE SEQUENCE dobavljaci_seq
    START WITH 3
    INCREMENT BY 1
    NO MINVALUE
    NO MAXVALUE
    CACHE 1;


ALTER TABLE dobavljaci_seq OWNER TO epc;

--
-- Name: dobavljaci; Type: TABLE; Schema: public; Owner: epc
--

CREATE TABLE dobavljaci (
    id integer DEFAULT nextval('dobavljaci_seq'::regclass) NOT NULL,
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
-- Name: mjere_kol; Type: TABLE; Schema: public; Owner: epc
--

CREATE TABLE mjere_kol (
    id integer NOT NULL,
    mjera character varying(16) NOT NULL,
    skraceno character varying(8) NOT NULL
);


ALTER TABLE mjere_kol OWNER TO epc;

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
-- Name: skladista_seq; Type: SEQUENCE; Schema: public; Owner: epc
--

CREATE SEQUENCE skladista_seq
    START WITH 2
    INCREMENT BY 1
    NO MINVALUE
    NO MAXVALUE
    CACHE 1;


ALTER TABLE skladista_seq OWNER TO epc;

--
-- Name: skladista; Type: TABLE; Schema: public; Owner: epc
--

CREATE TABLE skladista (
    id integer DEFAULT nextval('skladista_seq'::regclass) NOT NULL,
    vrijeme_od timestamp without time zone DEFAULT now() NOT NULL,
    vrijeme_do timestamp without time zone DEFAULT 'infinity'::timestamp without time zone NOT NULL,
    lokacija character varying(64),
    telefon character varying(24),
    faks character varying(24),
    oznaka character varying(128) NOT NULL
);


ALTER TABLE skladista OWNER TO epc;

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
    biljeska character varying(1024),
    mjera integer
);


ALTER TABLE stanje OWNER TO epc;

--
-- Name: stitovi; Type: TABLE; Schema: public; Owner: epc
--

CREATE TABLE stitovi (
    materijal character varying(128),
    gustoca real,
    debljina_zida real,
    gustoca_mag_toka real,
    faktor_zastite double precision,
    efikasnost_zastite real,
    krit_temp_zero real,
    krit_temp_srednje real,
    unut_promjer real,
    duljina real
)
INHERITS (materijali);


ALTER TABLE stitovi OWNER TO epc;

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
-- Name: id; Type: DEFAULT; Schema: public; Owner: epc
--

ALTER TABLE ONLY dimenzije ALTER COLUMN id SET DEFAULT nextval('dimenzije_id_seq'::regclass);


--
-- Name: vrijeme_od; Type: DEFAULT; Schema: public; Owner: epc
--

ALTER TABLE ONLY shim_zavojnice ALTER COLUMN vrijeme_od SET DEFAULT now();


--
-- Name: vrijeme_dobavljaca; Type: DEFAULT; Schema: public; Owner: epc
--

ALTER TABLE ONLY shim_zavojnice ALTER COLUMN vrijeme_dobavljaca SET DEFAULT now();


--
-- Name: vrijeme_do; Type: DEFAULT; Schema: public; Owner: epc
--

ALTER TABLE ONLY shim_zavojnice ALTER COLUMN vrijeme_do SET DEFAULT 'infinity'::timestamp without time zone;


--
-- Name: vrijeme_od; Type: DEFAULT; Schema: public; Owner: epc
--

ALTER TABLE ONLY stitovi ALTER COLUMN vrijeme_od SET DEFAULT now();


--
-- Name: vrijeme_dobavljaca; Type: DEFAULT; Schema: public; Owner: epc
--

ALTER TABLE ONLY stitovi ALTER COLUMN vrijeme_dobavljaca SET DEFAULT now();


--
-- Name: vrijeme_do; Type: DEFAULT; Schema: public; Owner: epc
--

ALTER TABLE ONLY stitovi ALTER COLUMN vrijeme_do SET DEFAULT 'infinity'::timestamp without time zone;


--
-- Name: vrijeme_od; Type: DEFAULT; Schema: public; Owner: epc
--

ALTER TABLE ONLY supravodici ALTER COLUMN vrijeme_od SET DEFAULT now();


--
-- Name: vrijeme_dobavljaca; Type: DEFAULT; Schema: public; Owner: epc
--

ALTER TABLE ONLY supravodici ALTER COLUMN vrijeme_dobavljaca SET DEFAULT now();


--
-- Name: vrijeme_do; Type: DEFAULT; Schema: public; Owner: epc
--

ALTER TABLE ONLY supravodici ALTER COLUMN vrijeme_do SET DEFAULT 'infinity'::timestamp without time zone;


--
-- Name: vrijeme_od; Type: DEFAULT; Schema: public; Owner: epc
--

ALTER TABLE ONLY trake ALTER COLUMN vrijeme_od SET DEFAULT now();


--
-- Name: vrijeme_dobavljaca; Type: DEFAULT; Schema: public; Owner: epc
--

ALTER TABLE ONLY trake ALTER COLUMN vrijeme_dobavljaca SET DEFAULT now();


--
-- Name: vrijeme_do; Type: DEFAULT; Schema: public; Owner: epc
--

ALTER TABLE ONLY trake ALTER COLUMN vrijeme_do SET DEFAULT 'infinity'::timestamp without time zone;


--
-- Data for Name: dimenzije; Type: TABLE DATA; Schema: public; Owner: epc
--

COPY dimenzije (id, tablica_podtip, "1", "2", "3", "4", "5", "6", "7", "8", "9", "10") FROM stdin;
1	supravodici	\N	mm	mm	\N	\N	A	A	A	A	mikron
2	shim_zavojnice	\N	A	\N	mm	T	\N	\N	\N	\N	\N
4	stitovi	\N	\N	g/cm3	mm	mT	\N	dB	K	K	mm
3	trake	\N	mm	mm	\N	\N	A	\N	\N	\N	\N
\.


--
-- Name: dimenzije_id_seq; Type: SEQUENCE SET; Schema: public; Owner: epc
--

SELECT pg_catalog.setval('dimenzije_id_seq', 4, true);


--
-- Data for Name: dobavljaci; Type: TABLE DATA; Schema: public; Owner: epc
--

COPY dobavljaci (id, vrijeme_od, naziv, vrijeme_do, adresa, telefon, telefon2, "e-mail") FROM stdin;
1	2018-01-08 04:19:05.176647	D1	2018-01-08 04:19:05.176647	\N	\N	\N	\N
1	2018-01-10 15:22:07.552919	Supercon Inc.	infinity	\N	\N	\N	\N
2	2018-02-18 21:24:10.28095	Fujikura	infinity	Leatherhead Road,Chessington,Surrey,KT9 2NY, UK	+44 (0) 20 8240 2000	\N	sales@fujikura.co.uk
11	2018-03-02 16:36:10.276518	Test8f	2018-03-02 16:36:24.036245				
11	2018-03-02 16:36:24.036245	Test8g	2018-03-02 16:36:33.684442				
1	2018-01-10 15:18:13.241298	Supercon	2018-01-10 15:22:07.552919	\N	\N	\N	\N
11	2018-03-02 16:36:33.684442	Test8h	2018-03-02 16:36:43.923849				
11	2018-03-02 16:36:43.923849	Test8i	2018-03-02 16:36:52.555846				
11	2018-03-02 16:36:52.555846	Test8j	2018-03-02 16:37:12.172635				
11	2018-03-02 16:37:12.172635	Test8k	2018-03-03 00:26:19.606249				
11	2018-03-03 00:26:19.606249	Test8l	2018-03-03 00:26:30.037833				
11	2018-03-03 00:26:30.037833	Test8m	2018-03-03 00:26:45.49405				
11	2018-03-03 00:26:45.49405	Test8n	2018-03-03 00:27:10.990497				
8	2018-03-02 15:15:15.952814	Test5	2018-03-03 02:38:47.29669				
8	2018-03-03 02:38:47.29669	Test5a	infinity				
11	2018-03-03 00:27:10.990497	Test80	2018-03-23 23:29:14.305623				
10	2018-03-02 15:16:16.770596	Test7	2018-03-23 23:29:59.632333				
10	2018-03-23 23:29:59.632333	Test7a	infinity				
9	2018-03-02 15:15:46.024675	Test6	2018-03-23 23:30:44.976959				
9	2018-03-23 23:30:44.976959	Test6	infinity	Adresna 1			
11	2018-03-23 23:29:14.305623	Test80a	2018-03-23 23:31:08.931857				
1	2018-01-08 04:19:37.661234	D2	2018-01-10 15:22:07.552919	Pavlinska bb, Varazdin	\N	\N	\N
3	2018-03-01 09:03:49.464044	Can superconductors	infinity	Ringhofferova 66, 251 68  Kamenice, Czech Republic	+420 323 619 695	+420 323 619 697	info@can-superconductors.com
4	2018-03-02 15:14:35.152021	Test1	infinity				
5	2018-03-02 15:14:44.681835	Test2	infinity				
6	2018-03-02 15:14:53.649696	Test3	infinity				
7	2018-03-02 15:15:05.489322	Test4	infinity				
11	2018-03-02 15:16:30.248802	Test8	2018-03-02 16:35:08.556278				
11	2018-03-02 16:35:08.556278	Test8a	2018-03-02 16:35:23.764166				
11	2018-03-02 16:35:23.764166	Test8b	2018-03-02 16:35:38.876491				
11	2018-03-02 16:35:38.876491	Test8c	2018-03-02 16:35:50.076217				
11	2018-03-02 16:35:50.076217	Test8d	2018-03-02 16:36:01.428665				
11	2018-03-02 16:36:01.428665	Test8e	2018-03-02 16:36:10.276518				
11	2018-03-23 23:31:39.567604	Test80ab	2018-04-10 14:27:14.156936	Adresna X			
11	2018-04-10 14:27:14.156936	Test80ab_	2018-04-10 14:29:11.884488	Adresna X			
11	2018-04-10 14:29:11.884488	Test80ab_	infinity	Adresna X			
\.


--
-- Name: dobavljaci_seq; Type: SEQUENCE SET; Schema: public; Owner: epc
--

SELECT pg_catalog.setval('dobavljaci_seq', 3, false);


--
-- Data for Name: materijali; Type: TABLE DATA; Schema: public; Owner: epc
--

COPY materijali (id, vrijeme_od, dobavljac, vrijeme_dobavljaca, vrijeme_do, naziv) FROM stdin;
\.


--
-- Data for Name: mjere_kol; Type: TABLE DATA; Schema: public; Owner: epc
--

COPY mjere_kol (id, mjera, skraceno) FROM stdin;
1	komada	kom.
2	kilograma	kg
3	tona	t
4	metara	m
\.


--
-- Data for Name: shim_zavojnice; Type: TABLE DATA; Schema: public; Owner: epc
--

COPY shim_zavojnice (id, vrijeme_od, dobavljac, vrijeme_dobavljaca, vrijeme_do, naziv, tip, max_struja, sparivanje, promjer, jakost) FROM stdin;
14	2018-01-10 17:43:44.69614	1	2018-01-10 15:22:07.552919	infinity	SH-Z1	Z1	25	Preko glavne zavojnice	10	10.8000002
15	2018-01-10 17:44:21.360294	1	2018-01-10 15:22:07.552919	infinity	SH-Z1	Z2	25	Preko glavne zavojnice	10	4.73999977
23	2018-03-09 12:01:52.977178	1	2018-01-10 15:22:07.552919	2018-03-09 12:02:39.65731	TEST	testni	\N	\N	\N	\N
23	2018-03-09 12:02:39.65731	1	2018-01-10 15:22:07.552919	2018-03-09 12:02:56.078411	TEST_A	testni2	\N	\N	\N	\N
32	2018-04-08 20:01:03.599379	1	2018-01-10 15:22:07.552919	2018-04-08 20:36:24.029337	Test	Tip_test	1000	\N	20	10
32	2018-04-08 20:36:24.029337	1	2018-01-10 15:22:07.552919	2018-04-08 20:36:42.904597	Test	Tip_test	1005	sa glavnom zavojnicom	21	9
37	2018-04-09 02:08:10.55935	1	2018-01-10 15:22:07.552919	2018-04-09 02:08:22.270133	Test	Tip-test	1000	sa glavnom zavojnicom	20	2
37	2018-04-09 02:08:22.270133	3	2018-03-01 09:03:49.464044	infinity	Test	Tip-test	1000	sa glavnom zavojnicom	20	2
\.


--
-- Data for Name: skladista; Type: TABLE DATA; Schema: public; Owner: epc
--

COPY skladista (id, vrijeme_od, vrijeme_do, lokacija, telefon, faks, oznaka) FROM stdin;
1	2018-01-08 04:02:30.817858	2018-01-08 04:02:37.849104	\N	\N	\N	S1
1	2018-01-08 04:02:37.849104	2018-01-08 04:04:59.241159	\N	\N	\N	S1
1	2018-01-08 04:05:52.83625	2018-01-08 04:05:58.014813	\N	\N	\N	S1
1	2018-01-08 04:09:40.086676	2018-01-08 04:09:40.086676	\N	\N	\N	S1
1	2018-01-08 04:11:34.78791	2018-01-08 04:11:34.78791	\N	\N	\N	S1
1	2018-01-08 04:12:29.72161	2018-01-08 04:12:35.660325	\N	\N	\N	S1
1	2018-01-08 04:12:35.660325	2018-01-08 04:12:35.660325	\N	\N	\N	S1
1	2018-01-08 04:12:47.197856	2018-01-08 04:12:47.197856	\N	\N	\N	S1
1	2018-01-08 04:12:56.027086	2018-03-03 02:34:14.009188	\N	\N	\N	S1
1	2018-03-03 02:34:14.009188	infinity				S1a
2	2018-03-03 02:35:49.561026	2018-03-03 02:36:18.097403	Testna 1			S test
2	2018-03-03 02:36:18.097403	2018-03-03 02:36:42.5847	Testna 1a			S testa
2	2018-03-03 02:36:42.5847	2018-03-03 02:37:43.441374	Testna 1b			S testb
2	2018-03-03 02:37:43.441374	infinity	Testna 1c			S testc
\.


--
-- Name: skladista_seq; Type: SEQUENCE SET; Schema: public; Owner: epc
--

SELECT pg_catalog.setval('skladista_seq', 2, false);


--
-- Data for Name: stanje; Type: TABLE DATA; Schema: public; Owner: epc
--

COPY stanje (skladiste, vrijeme_skladista, materijal, vrijeme_materijala, vrijeme_od, kolicina, vrijeme_do, biljeska, mjera) FROM stdin;
1	2018-01-08 04:12:56.027086	5	2018-01-10 14:57:17.900401	2018-02-18 09:02:59.226027	5	infinity	\N	1
1	2018-03-03 02:34:14.009188	10	2018-01-10 15:02:13.809029	2018-04-15 04:38:29.158616	0	infinity	\N	1
1	2018-03-03 02:34:14.009188	14	2018-01-10 17:43:44.69614	2018-04-15 04:38:39.097761	0	infinity	\N	1
1	2018-03-03 02:34:14.009188	21	2018-03-01 09:53:10.027462	2018-04-15 04:38:43.122779	0	infinity	\N	1
2	2018-03-03 02:37:43.441374	21	2018-03-01 09:53:10.027462	2018-04-15 04:38:47.788947	0	infinity	\N	1
2	2018-03-03 02:37:43.441374	34	2018-04-09 23:55:23.985294	2018-04-15 04:43:05.734445	0	infinity	\N	1
1	2018-03-03 02:34:14.009188	30	2018-04-09 00:57:40.278151	2018-04-15 04:43:39.517958	0	2018-04-15 04:54:48.959585	\N	1
2	2018-03-03 02:37:43.441374	30	2018-04-09 00:57:40.278151	2018-04-15 04:43:36.622304	0	2018-04-15 15:22:14.223646	\N	1
1	2018-03-03 02:34:14.009188	30	2018-04-09 00:57:40.278151	2018-04-15 04:54:48.959585	5	2018-04-15 15:22:14.223646	\N	1
1	2018-03-03 02:34:14.009188	30	2018-04-09 00:57:40.278151	2018-04-15 15:24:03.296591	10	2018-04-15 16:27:50.607495	\N	1
1	2018-03-03 02:34:14.009188	30	2018-04-15 16:26:38.884239	2018-04-15 16:27:50.607495	12	infinity	\N	1
\.


--
-- Data for Name: stitovi; Type: TABLE DATA; Schema: public; Owner: epc
--

COPY stitovi (id, vrijeme_od, dobavljac, vrijeme_dobavljaca, vrijeme_do, naziv, materijal, gustoca, debljina_zida, gustoca_mag_toka, faktor_zastite, efikasnost_zastite, krit_temp_zero, krit_temp_srednje, unut_promjer, duljina) FROM stdin;
21	2018-03-01 09:52:59.254188	3	2018-03-01 09:03:49.464044	2018-03-01 09:53:10.027462	CST-10/80	Bi1.8Pb0.26Sr2Ca2Cu3O10+x (2223 phase)	\N	\N	\N	\N	\N	\N	\N	\N	\N
21	2018-03-01 09:53:10.027462	3	2018-03-01 09:03:49.464044	infinity	CST-10/80	Bi1.8Pb0.26Sr2Ca2Cu3O10+x (2223 phase)	5	1.5	5	10000000	120	108	110	10	80
34	2018-04-08 23:15:37.27342	2	2018-02-18 21:24:10.28095	2018-04-08 23:16:21.90485	Test	Materijal-test	100	1	1000	0.900000000000000022	100	20.7000008	15	1	10000
34	2018-04-08 23:16:21.90485	2	2018-02-18 21:24:10.28095	2018-04-08 23:16:42.60151	Test	Materijal-test1	120	1.20000005	1000	0.910000000000000031	101	20.7199993	16	1.10000002	20000
39	2018-04-09 02:09:39.861089	2	2018-02-18 21:24:10.28095	infinity	Test2	Mat-test2	\N	\N	\N	\N	\N	\N	\N	\N	\N
34	2018-04-08 23:16:55.728682	2	2018-02-18 21:24:10.28095	2018-04-09 02:48:21.339595	Test	Materijal-test1	120	1.20000005	1000	0.910000000000000031	101	20.7199993	16	1.12	20000
34	2018-04-09 02:48:21.339595	2	2018-02-18 21:24:10.28095	2018-04-09 23:55:23.985294	Test	Materijal-test1	120	1.20000005	1000	0.910000000000000031	102	20.7199993	16	1.12	20000
34	2018-04-09 23:55:23.985294	2	2018-02-18 21:24:10.28095	infinity	Test	Materijal-test1a	120	1.20000005	1000	0.910000000000000031	102	20.7199993	16	1.12	20000
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
30	2018-03-30 14:41:24.539929	1	2018-01-10 15:22:07.552919	2018-03-30 14:42:23.680658	Test	Tip 1	\N	\N	\N	\N	\N	\N	\N	\N	\N
30	2018-03-30 14:42:23.680658	1	2018-01-10 15:22:07.552919	2018-03-30 14:43:22.38518	Test1	Tip 1	\N	\N	\N	\N	\N	\N	\N	\N	\N
30	2018-03-30 14:43:22.38518	1	2018-01-10 15:22:07.552919	2018-03-30 15:00:27.920585	Testb	Tip 1	\N	\N	\N	\N	\N	\N	\N	\N	\N
30	2018-03-30 15:00:27.920585	1	2018-01-10 15:22:07.552919	2018-03-30 16:13:06.112477	Test1a	T1a	\N	\N	\N	\N	\N	\N	\N	\N	\N
30	2018-03-30 16:13:06.112477	1	2018-01-10 15:22:07.552919	2018-03-30 16:55:11.375911	Test1ab	T1ab	5	6	13	10000	102	103	105	\N	17
30	2018-03-30 16:55:11.375911	1	2018-01-10 15:22:07.552919	2018-03-30 17:08:58.79196	Test1a	T1a	5	6	13	10000	102	103	105	107	17
30	2018-03-30 17:08:58.79196	1	2018-01-10 15:22:07.552919	2018-03-30 17:10:22.671961	fg		5	6	13	10000	102	103	105	107	17
30	2018-03-30 17:10:22.671961	1	2018-01-10 15:22:07.552919	2018-03-30 17:12:40.728211	Test1	Tip1	5	6	13	10000	102	103	105	107	17
30	2018-03-30 17:12:40.728211	1	2018-01-10 15:22:07.552919	2018-03-30 17:24:02.352779	Test1a	Tip1	5	6	13	10000	102	103	105	107	17
30	2018-03-30 17:24:02.352779	1	2018-01-10 15:22:07.552919	2018-03-30 18:55:54.117187	Test1	Tip1	5.5	6	13	10000	102	103	105	107	17
31	2018-03-30 19:25:42.675618	2	2018-02-18 21:24:10.28095	2018-03-30 19:33:32.587286	Test2	Tip2	3	3.5	40	1000000	101	102	103	104	20
31	2018-03-30 19:33:32.587286	2	2018-02-18 21:24:10.28095	2018-03-31 01:57:17.546921	Test2	Tip2	3	3.5	40	1000000	101	102	103	104	21
31	2018-03-31 02:26:30.235938	2	2018-02-18 21:24:10.28095	2018-03-31 02:27:03.5801	Test2_a	Tip2_a	\N	\N	\N	\N	\N	\N	\N	\N	\N
31	2018-03-31 03:15:11.564853	2	2018-02-18 21:24:10.28095	2018-03-31 03:19:44.590395	Test2_a	Tip2_a	3	\N	\N	\N	\N	\N	\N	\N	\N
30	2018-03-30 18:55:54.117187	1	2018-01-10 15:22:07.552919	2018-04-08 23:37:25.734148	Test1	Tip1	5.5	6	13	10000	102	103	105	107	18
30	2018-04-08 23:37:25.734148	1	2018-01-10 15:22:07.552919	2018-04-08 23:37:52.094071	Test1	Tip1	5.5	6	13	10000	102	103	105	107	18
30	2018-04-08 23:37:52.094071	1	2018-01-10 15:22:07.552919	2018-04-08 23:39:13.765888	Test1	Tip1	5.5	6	13	10000	102	103	105	107	18
30	2018-04-08 23:39:13.765888	1	2018-01-10 15:22:07.552919	2018-04-08 23:47:58.516689	Test1	Tip1	5.5	6	13	10000	102	103	105	107	18
30	2018-04-08 23:47:58.516689	1	2018-01-10 15:22:07.552919	2018-04-09 00:57:40.278151	Test1	Tip1	5.5	6	13	10000	102	103	105	107	18
35	2018-04-09 01:46:14.786952	3	2018-03-01 09:03:49.464044	2018-04-09 01:46:30.866661	Test2	Tip-test2	2	3	23	100	\N	\N	\N	\N	12
35	2018-04-09 01:46:30.866661	1	2018-01-10 15:22:07.552919	2018-04-09 01:46:47.674549	Test2	Tip-test2	2	3	23	100	\N	\N	\N	\N	12
36	2018-04-09 02:02:10.452794	1	2018-01-10 15:22:07.552919	2018-04-09 02:02:19.532011	Test3	Tip-test3	\N	\N	\N	\N	\N	\N	\N	\N	\N
36	2018-04-09 02:02:19.532011	3	2018-03-01 09:03:49.464044	2018-04-09 02:20:20.376099	Test3	Tip-test3	\N	\N	\N	\N	\N	\N	\N	\N	\N
36	2018-04-09 02:20:20.376099	2	2018-02-18 21:24:10.28095	2018-04-09 02:24:02.255079	Test3	Tip-test3	\N	\N	\N	\N	\N	\N	\N	\N	\N
36	2018-04-09 02:24:02.255079	3	2018-03-01 09:03:49.464044	2018-04-09 02:24:25.353223	Test3	Tip-test3	\N	\N	\N	\N	\N	\N	\N	\N	\N
35	2018-04-09 01:46:47.674549	3	2018-03-01 09:03:49.464044	2018-04-09 02:24:45.535928	Test2	Tip-test2	2	3	23	100	\N	\N	\N	\N	12
30	2018-04-09 00:57:40.278151	3	2018-03-01 09:03:49.464044	2018-04-15 16:26:38.884239	Test1	Tip1	5.5	6	13	10000	102	103	105	107	18
30	2018-04-15 16:26:38.884239	3	2018-03-01 09:03:49.464044	infinity	test_P	Tip1	5.5	6	13	10000	102	103	105	107	18
\.


--
-- Data for Name: trake; Type: TABLE DATA; Schema: public; Owner: epc
--

COPY trake (id, vrijeme_od, dobavljac, vrijeme_dobavljaca, vrijeme_do, naziv, sirina, debljina, supstrat, stabilizator, krit_struja) FROM stdin;
17	2018-02-18 21:38:59.641284	2	2018-02-18 21:24:10.28095	infinity	FYSC-SC05	5	0.159999996	75	75	250
18	2018-02-18 21:39:41.153549	2	2018-02-18 21:24:10.28095	infinity	FYSC-SC10	10	0.159999996	75	75	500
19	2018-02-18 21:40:23.537238	2	2018-02-18 21:24:10.28095	infinity	FYSC-S05	5	0.0799999982	75	0	250
20	2018-02-18 21:40:48.152601	2	2018-02-18 21:24:10.28095	infinity	FYSC-S10	10	0.0799999982	75	0	500
33	2018-04-08 22:25:23.390232	3	2018-03-01 09:03:49.464044	2018-04-08 22:31:36.086557	Test	12	0.25	\N	\N	1000
33	2018-04-08 22:31:36.086557	3	2018-03-01 09:03:49.464044	2018-04-09 02:08:43.333279	Test	16	0.270000011	\N	\N	1600
33	2018-04-09 02:08:43.333279	1	2018-01-10 15:22:07.552919	infinity	Test	16	0.270000011	\N	\N	1600
38	2018-04-09 02:09:07.670074	1	2018-01-10 15:22:07.552919	2018-04-09 02:09:16.62307	Test2	\N	\N	\N	\N	\N
38	2018-04-09 02:09:16.62307	3	2018-03-01 09:03:49.464044	infinity	Test2	\N	\N	\N	\N	\N
\.


--
-- Name: dimenzije_pkey; Type: CONSTRAINT; Schema: public; Owner: epc
--

ALTER TABLE ONLY dimenzije
    ADD CONSTRAINT dimenzije_pkey PRIMARY KEY (id);


--
-- Name: dobavljaci_pkey; Type: CONSTRAINT; Schema: public; Owner: epc
--

ALTER TABLE ONLY dobavljaci
    ADD CONSTRAINT dobavljaci_pkey PRIMARY KEY (id, vrijeme_od);


--
-- Name: materijali_pkey; Type: CONSTRAINT; Schema: public; Owner: epc
--

ALTER TABLE ONLY materijali
    ADD CONSTRAINT materijali_pkey PRIMARY KEY (id, vrijeme_od);


--
-- Name: mjere_kol_primary_key; Type: CONSTRAINT; Schema: public; Owner: epc
--

ALTER TABLE ONLY mjere_kol
    ADD CONSTRAINT mjere_kol_primary_key PRIMARY KEY (id);


--
-- Name: skladista_pkey; Type: CONSTRAINT; Schema: public; Owner: epc
--

ALTER TABLE ONLY skladista
    ADD CONSTRAINT skladista_pkey PRIMARY KEY (id, vrijeme_od);


--
-- Name: stanje_pkey; Type: CONSTRAINT; Schema: public; Owner: epc
--

ALTER TABLE ONLY stanje
    ADD CONSTRAINT stanje_pkey PRIMARY KEY (skladiste, vrijeme_od, materijal, vrijeme_skladista, vrijeme_materijala);


--
-- Name: dobavljaci_okidac; Type: TRIGGER; Schema: public; Owner: epc
--

CREATE TRIGGER dobavljaci_okidac BEFORE INSERT OR DELETE OR UPDATE ON dobavljaci FOR EACH ROW EXECUTE PROCEDURE azuriranje_dobavljaca();


--
-- Name: materijali_okidac; Type: TRIGGER; Schema: public; Owner: epc
--

CREATE TRIGGER materijali_okidac BEFORE INSERT OR DELETE OR UPDATE ON materijali FOR EACH ROW EXECUTE PROCEDURE azuriranje_materijala();


--
-- Name: shim_zavojnice_okidac; Type: TRIGGER; Schema: public; Owner: epc
--

CREATE TRIGGER shim_zavojnice_okidac BEFORE INSERT OR DELETE OR UPDATE ON shim_zavojnice FOR EACH ROW EXECUTE PROCEDURE azuriranje_shim_zavojnice();


--
-- Name: skladista_okidac; Type: TRIGGER; Schema: public; Owner: epc
--

CREATE TRIGGER skladista_okidac BEFORE INSERT OR DELETE OR UPDATE ON skladista FOR EACH ROW EXECUTE PROCEDURE azuriranje_skladista();


--
-- Name: stanje_okidac; Type: TRIGGER; Schema: public; Owner: epc
--

CREATE TRIGGER stanje_okidac BEFORE INSERT OR DELETE OR UPDATE ON stanje FOR EACH ROW EXECUTE PROCEDURE azuriranje_stanja();


--
-- Name: stitovi_okidac; Type: TRIGGER; Schema: public; Owner: epc
--

CREATE TRIGGER stitovi_okidac BEFORE INSERT OR DELETE OR UPDATE ON stitovi FOR EACH ROW EXECUTE PROCEDURE azuriranje_stitova();


--
-- Name: supravodici_okidac; Type: TRIGGER; Schema: public; Owner: epc
--

CREATE TRIGGER supravodici_okidac BEFORE INSERT OR DELETE OR UPDATE ON supravodici FOR EACH ROW EXECUTE PROCEDURE azuriranje_supravodica();


--
-- Name: trake_okidac; Type: TRIGGER; Schema: public; Owner: epc
--

CREATE TRIGGER trake_okidac BEFORE INSERT OR DELETE OR UPDATE ON trake FOR EACH ROW EXECUTE PROCEDURE azuriranje_trake();


--
-- Name: Ref_materijali_to_dobavljaci; Type: FK CONSTRAINT; Schema: public; Owner: epc
--

ALTER TABLE ONLY materijali
    ADD CONSTRAINT "Ref_materijali_to_dobavljaci" FOREIGN KEY (dobavljac, vrijeme_dobavljaca) REFERENCES dobavljaci(id, vrijeme_od);


--
-- Name: Ref_stanje_to_skladista; Type: FK CONSTRAINT; Schema: public; Owner: epc
--

ALTER TABLE ONLY stanje
    ADD CONSTRAINT "Ref_stanje_to_skladista" FOREIGN KEY (skladiste, vrijeme_skladista) REFERENCES skladista(id, vrijeme_od);


--
-- Name: stanje_mjera_fkey; Type: FK CONSTRAINT; Schema: public; Owner: epc
--

ALTER TABLE ONLY stanje
    ADD CONSTRAINT stanje_mjera_fkey FOREIGN KEY (mjera) REFERENCES mjere_kol(id);


--
-- Name: SCHEMA public; Type: ACL; Schema: -; Owner: postgres
--

REVOKE ALL ON SCHEMA public FROM PUBLIC;
REVOKE ALL ON SCHEMA public FROM postgres;
GRANT ALL ON SCHEMA public TO postgres;
GRANT ALL ON SCHEMA public TO PUBLIC;


--
-- PostgreSQL database dump complete
--

