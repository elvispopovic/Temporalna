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
    mjera character varying(16),
    skraceno character varying(8)
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
1	2018-04-13 14:16:06.754331	Dobavljač testni	infinity	Pavlinska 1, Varaždin			elvpopovi@foi.hr
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
\.


--
-- Data for Name: skladista; Type: TABLE DATA; Schema: public; Owner: epc
--

COPY skladista (id, vrijeme_od, vrijeme_do, lokacija, telefon, faks, oznaka) FROM stdin;
\.


--
-- Name: skladista_seq; Type: SEQUENCE SET; Schema: public; Owner: epc
--

SELECT pg_catalog.setval('skladista_seq', 2, false);


--
-- Data for Name: stanje; Type: TABLE DATA; Schema: public; Owner: epc
--

COPY stanje (skladiste, vrijeme_skladista, materijal, vrijeme_materijala, vrijeme_od, kolicina, vrijeme_do, biljeska, mjera) FROM stdin;
\.


--
-- Data for Name: stitovi; Type: TABLE DATA; Schema: public; Owner: epc
--

COPY stitovi (id, vrijeme_od, dobavljac, vrijeme_dobavljaca, vrijeme_do, naziv, materijal, gustoca, debljina_zida, gustoca_mag_toka, faktor_zastite, efikasnost_zastite, krit_temp_zero, krit_temp_srednje, unut_promjer, duljina) FROM stdin;
\.


--
-- Data for Name: supravodici; Type: TABLE DATA; Schema: public; Owner: epc
--

COPY supravodici (id, vrijeme_od, dobavljac, vrijeme_dobavljaca, vrijeme_do, naziv, tip, cisti_promjer, promjer_izolator, broj_niti, cu_sc, "kriticna_struja3T", "kriticna_struja5T", "kriticna_struja7T", "kriticna_struja9T", promjer_niti) FROM stdin;
1	2018-04-14 17:34:37.339353	1	2018-04-13 14:16:06.754331	infinity	Supravodič testni	Tip testni	8	9	23	100000	107	106	105	104	20
\.


--
-- Data for Name: trake; Type: TABLE DATA; Schema: public; Owner: epc
--

COPY trake (id, vrijeme_od, dobavljac, vrijeme_dobavljaca, vrijeme_do, naziv, sirina, debljina, supstrat, stabilizator, krit_struja) FROM stdin;
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
-- Name: mjere_kol_pkey; Type: CONSTRAINT; Schema: public; Owner: epc
--

ALTER TABLE ONLY mjere_kol
    ADD CONSTRAINT mjere_kol_pkey PRIMARY KEY (id);


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

