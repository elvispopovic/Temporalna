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

 redak audit%ROWTYPE;

BEGIN

 dubina = pg_trigger_depth();

IF TG_OP='UPDATE' THEN

 IF dubina=3 OR dubina =2 THEN RETURN NEW; 

 ELSIF OLD.vrijeme_do='infinity'::TIMESTAMP THEN

  INSERT INTO dobavljaci(id,naziv,vrijeme_do,adresa,telefon,telefon2,"e-mail") 

  VALUES(NEW.id, NEW.naziv, 'infinity'::TIMESTAMP, NEW.adresa, NEW.telefon, NEW.telefon2, NEW."e-mail");

  INSERT INTO audit(vrijeme,tablica,id_tablice,dogadjaj,korisnik) 

  VALUES(current_timestamp,'dobavljaci',NEW.id,'UPDATE',current_user);

 END IF;

 RETURN NULL;

END IF;

IF TG_OP='INSERT' THEN

 UPDATE dobavljaci SET vrijeme_do=current_timestamp WHERE 

  id=NEW.id AND vrijeme_do='infinity'::TIMESTAMP;

  INSERT INTO audit(vrijeme,tablica,id_tablice,dogadjaj,korisnik) 

  VALUES(current_timestamp,'dobavljaci',NEW.id,'INSERT',current_user);

  RETURN NEW;

END IF;

IF TG_OP='DELETE' THEN

 UPDATE dobavljaci SET vrijeme_do=current_timestamp WHERE

  id=OLD.id AND vrijeme_od = OLD.vrijeme_od AND 

  vrijeme_do='infinity'::TIMESTAMP;

  SELECT * INTO redak FROM audit WHERE vrijeme=current_timestamp;

  IF NOT FOUND THEN

  INSERT INTO audit(vrijeme,tablica,id_tablice,dogadjaj,korisnik) 

  VALUES(current_timestamp,'dobavljaci',OLD.id,'DELETE',current_user);

  END IF;

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

redak_audit audit%ROWTYPE;

dubina INTEGER;

BEGIN

dubina = pg_trigger_depth();

IF TG_OP='UPDATE' THEN

 IF dubina=3 OR dubina =2 THEN RETURN NEW; 

 ELSIF OLD.vrijeme_do='infinity'::TIMESTAMP THEN

  INSERT INTO materijali(id,dobavljac, vrijeme_dobavljaca,vrijeme_do,naziv)

  VALUES(NEW.id, NEW.dobavljac, NEW.vrijeme_dobavljaca, 'infinity'::TIMESTAMP, NEW.naziv);

  INSERT INTO audit(vrijeme,tablica,id_tablice,dogadjaj,korisnik) 

  VALUES(current_timestamp,'materijali',NEW.id,'UPDATE',current_user);  

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

 INSERT INTO audit(vrijeme,tablica,id_tablice,dogadjaj,korisnik) 

  VALUES(current_timestamp,'materijali',NEW.id,'INSERT',current_user);

 RETURN NEW;

END IF;

IF TG_OP='DELETE' THEN

 UPDATE materijali SET vrijeme_do=current_timestamp WHERE

  id=OLD.id AND vrijeme_od = OLD.vrijeme_od AND 

  vrijeme_do='infinity'::TIMESTAMP;

  SELECT * INTO redak_audit FROM audit WHERE vrijeme=current_timestamp;

  IF NOT FOUND THEN

  INSERT INTO audit(vrijeme,tablica,id_tablice,dogadjaj,korisnik) 

  VALUES(current_timestamp,'materijali',OLD.id,'DELETE',current_user);

  END IF;

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

redak_audit audit%ROWTYPE;

dubina INTEGER;

BEGIN

dubina = pg_trigger_depth();

IF TG_OP='UPDATE' THEN

 IF dubina=3 OR dubina =2 THEN RETURN NEW; 

 ELSIF OLD.vrijeme_do='infinity'::TIMESTAMP THEN

  INSERT INTO shim_zavojnice(id,dobavljac, vrijeme_dobavljaca,vrijeme_do,naziv, tip,max_struja,sparivanje,promjer,jakost)

  VALUES(NEW.id, NEW.dobavljac, NEW.vrijeme_dobavljaca, 'infinity'::TIMESTAMP, NEW.naziv, NEW.tip, NEW.max_struja, 

  NEW.sparivanje, NEW.promjer, NEW.jakost);

  INSERT INTO audit(vrijeme,tablica,id_tablice,dogadjaj,korisnik) 

  VALUES(current_timestamp,'shim_zavojnice',NEW.id,'UPDATE',current_user); 

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

  INSERT INTO audit(vrijeme,tablica,id_tablice,dogadjaj,korisnik) 

  VALUES(current_timestamp,'shim_zavojnice',NEW.id,'INSERT',current_user);

 RETURN NEW;

END IF;

IF TG_OP='DELETE' THEN

 UPDATE shim_zavojnice SET vrijeme_do=current_timestamp WHERE

  id=OLD.id AND vrijeme_od = OLD.vrijeme_od AND 

  vrijeme_do='infinity'::TIMESTAMP;

  SELECT * INTO redak_audit FROM audit WHERE vrijeme=current_timestamp;

  IF NOT FOUND THEN

  INSERT INTO audit(vrijeme,tablica,id_tablice,dogadjaj,korisnik) 

  VALUES(current_timestamp,'shim_zavojnice',OLD.id,'DELETE',current_user);

  END IF;

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

 redak audit%ROWTYPE;

BEGIN

 dubina = pg_trigger_depth();

IF TG_OP='UPDATE' THEN

 IF dubina=3 OR dubina =2 THEN RETURN NEW; 

 ELSIF OLD.vrijeme_do='infinity'::TIMESTAMP THEN

  INSERT INTO skladista(id,vrijeme_do,lokacija,telefon,faks,oznaka) 

  VALUES(NEW.id, 'infinity'::TIMESTAMP, NEW.lokacija, NEW.telefon, NEW.faks, NEW.oznaka);

  INSERT INTO audit(vrijeme,tablica,id_tablice,dogadjaj,korisnik) 

  VALUES(current_timestamp,'skladista',NEW.id,'UPDATE',current_user);

 END IF;

 RETURN NULL;

END IF;

IF TG_OP='INSERT' THEN

 UPDATE skladista SET vrijeme_do=current_timestamp WHERE 

  id=NEW.id AND vrijeme_do='infinity'::TIMESTAMP;

  INSERT INTO audit(vrijeme,tablica,id_tablice,dogadjaj,korisnik) 

  VALUES(current_timestamp,'skladista',NEW.id,'INSERT',current_user);

  RETURN NEW;

END IF;

IF TG_OP='DELETE' THEN

 UPDATE skladista SET vrijeme_do=current_timestamp WHERE

  id=OLD.id AND vrijeme_od = OLD.vrijeme_od AND 

  vrijeme_do='infinity'::TIMESTAMP;

  SELECT * INTO redak FROM audit WHERE vrijeme=current_timestamp;

  IF NOT FOUND THEN

  INSERT INTO audit(vrijeme,tablica,id_tablice,dogadjaj,korisnik) 

  VALUES(current_timestamp,'skladista',OLD.id,'DELETE',current_user);

  END IF;

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

redak_audit audit%ROWTYPE;

dubina INTEGER;

BEGIN

 dubina = pg_trigger_depth();

 IF TG_OP='UPDATE' THEN

 IF dubina=3 OR dubina =2 THEN RETURN NEW; 

 ELSIF OLD.vrijeme_do='infinity'::TIMESTAMP THEN

  INSERT INTO stanje(skladiste,vrijeme_skladista,materijal,vrijeme_materijala,kolicina,mjera,vrijeme_do,biljeska) 

  VALUES(NEW.skladiste, NEW.vrijeme_skladista, NEW.materijal, NEW.vrijeme_materijala, NEW.kolicina, NEW.mjera, 'infinity'::TIMESTAMP, NEW.biljeska);

  INSERT INTO audit(vrijeme,tablica,id_tablice,id_tablice2,dogadjaj,korisnik) 

  VALUES(current_timestamp,'stanje',NEW.skladiste,NEW.materijal,'UPDATE',current_user); 

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

  INSERT INTO audit(vrijeme,tablica,id_tablice,id_tablice2,dogadjaj,korisnik) 

  VALUES(current_timestamp,'stanje',NEW.skladiste,NEW.materijal,'INSERT',current_user);

 RETURN NEW;

END IF;

IF TG_OP='DELETE' THEN

UPDATE stanje SET vrijeme_do=current_timestamp WHERE 

   materijal=OLD.materijal AND vrijeme_od = OLD.vrijeme_od AND

   skladiste=OLD.skladiste AND vrijeme_od = OLD.vrijeme_od AND 

   vrijeme_do='infinity'::TIMESTAMP;

 SELECT * INTO redak_audit FROM audit WHERE vrijeme=current_timestamp;

  IF NOT FOUND THEN

  INSERT INTO audit(vrijeme,tablica,id_tablice,id_tablice2,dogadjaj,korisnik) 

  VALUES(current_timestamp,'stanje',OLD.skladiste,OLD.materijal,'DELETE',current_user);

  END IF;

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

redak_audit audit%ROWTYPE;

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

  INSERT INTO audit(vrijeme,tablica,id_tablice,dogadjaj,korisnik) 

  VALUES(current_timestamp,'stitovi',NEW.id,'UPDATE',current_user);  

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

    INSERT INTO audit(vrijeme,tablica,id_tablice,dogadjaj,korisnik) 

  VALUES(current_timestamp,'stitovi',NEW.id,'INSERT',current_user);

 RETURN NEW;

END IF;

IF TG_OP='DELETE' THEN

 UPDATE stitovi SET vrijeme_do=current_timestamp WHERE

  id=OLD.id AND vrijeme_od = OLD.vrijeme_od AND 

  vrijeme_do='infinity'::TIMESTAMP;

  SELECT * INTO redak_audit FROM audit WHERE vrijeme=current_timestamp;

  IF NOT FOUND THEN

  INSERT INTO audit(vrijeme,tablica,id_tablice,dogadjaj,korisnik) 

  VALUES(current_timestamp,'stitovi',OLD.id,'DELETE',current_user);

  END IF;

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

redak_audit audit%ROWTYPE;

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

  INSERT INTO audit(vrijeme,tablica,id_tablice,dogadjaj,korisnik) 

  VALUES(current_timestamp,'supravodici',NEW.id,'UPDATE',current_user); 

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

  INSERT INTO audit(vrijeme,tablica,id_tablice,dogadjaj,korisnik) 

  VALUES(current_timestamp,'supravodici',NEW.id,'INSERT',current_user);

 RETURN NEW;

END IF;

IF TG_OP='DELETE' THEN

 UPDATE supravodici SET vrijeme_do=current_timestamp WHERE

  id=OLD.id AND vrijeme_od = OLD.vrijeme_od AND 

  vrijeme_do='infinity'::TIMESTAMP;

  SELECT * INTO redak_audit FROM audit WHERE vrijeme=current_timestamp;

  IF NOT FOUND THEN

  INSERT INTO audit(vrijeme,tablica,id_tablice,dogadjaj,korisnik) 

  VALUES(current_timestamp,'supravodici',OLD.id,'DELETE',current_user);

  END IF;

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

redak_audit audit%ROWTYPE;

dubina INTEGER;

BEGIN

dubina = pg_trigger_depth();

IF TG_OP='UPDATE' THEN

 IF dubina=3 OR dubina =2 THEN RETURN NEW; 

 ELSIF OLD.vrijeme_do='infinity'::TIMESTAMP THEN

  INSERT INTO trake(id,dobavljac, vrijeme_dobavljaca,vrijeme_do,naziv, sirina,debljina,supstrat,stabilizator,krit_struja)

  VALUES(NEW.id, NEW.dobavljac, NEW.vrijeme_dobavljaca, 'infinity'::TIMESTAMP, NEW.naziv, NEW.sirina, NEW.debljina, 

  NEW.supstrat, NEW.stabilizator, NEW.krit_struja);

  INSERT INTO audit(vrijeme,tablica,id_tablice,dogadjaj,korisnik) 

  VALUES(current_timestamp,'trake',NEW.id,'UPDATE',current_user); 

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

  INSERT INTO audit(vrijeme,tablica,id_tablice,dogadjaj,korisnik) 

  VALUES(current_timestamp,'trake',NEW.id,'INSERT',current_user);

 RETURN NEW;

END IF;

IF TG_OP='DELETE' THEN

 UPDATE trake SET vrijeme_do=current_timestamp WHERE

  id=OLD.id AND vrijeme_od = OLD.vrijeme_od AND 

  vrijeme_do='infinity'::TIMESTAMP;

  SELECT * INTO redak_audit FROM audit WHERE vrijeme=current_timestamp;

  IF NOT FOUND THEN

  INSERT INTO audit(vrijeme,tablica,id_tablice,dogadjaj,korisnik) 

  VALUES(current_timestamp,'trake',OLD.id,'DELETE',current_user);

  END IF;

  RETURN NULL;

END IF;

END;

$$;


ALTER FUNCTION public.azuriranje_trake() OWNER TO epc;

SET default_tablespace = '';

SET default_with_oids = false;

--
-- Name: _pgmdd_backup_audit_2018-02-05_11:59; Type: TABLE; Schema: public; Owner: epc
--

CREATE TABLE "_pgmdd_backup_audit_2018-02-05_11:59" (
    id integer,
    vrijeme timestamp without time zone,
    tablica character varying(32),
    dogadjaj character varying(64),
    korisnik character varying(32),
    id_tablice integer
);


ALTER TABLE "_pgmdd_backup_audit_2018-02-05_11:59" OWNER TO epc;

--
-- Name: audit; Type: TABLE; Schema: public; Owner: epc
--

CREATE TABLE audit (
    id integer NOT NULL,
    vrijeme timestamp without time zone NOT NULL,
    tablica character varying(32),
    dogadjaj character varying(64),
    korisnik character varying(32),
    id_tablice integer,
    id_tablice2 integer
);


ALTER TABLE audit OWNER TO epc;

--
-- Name: audit_seq; Type: SEQUENCE; Schema: public; Owner: epc
--

CREATE SEQUENCE audit_seq
    START WITH 1
    INCREMENT BY 1
    NO MINVALUE
    NO MAXVALUE
    CACHE 1;


ALTER TABLE audit_seq OWNER TO epc;

--
-- Name: audit_seq; Type: SEQUENCE OWNED BY; Schema: public; Owner: epc
--

ALTER SEQUENCE audit_seq OWNED BY audit.id;


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
-- Name: dobavljaci_temp; Type: TABLE; Schema: public; Owner: epc
--

CREATE TABLE dobavljaci_temp (
    id integer,
    vrijeme_od timestamp without time zone,
    naziv character varying(64),
    vrijeme_do timestamp without time zone,
    adresa character varying(64),
    telefon character varying(24),
    telefon2 character varying(24),
    "e-mail" character varying(128)
);


ALTER TABLE dobavljaci_temp OWNER TO epc;

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

ALTER TABLE ONLY audit ALTER COLUMN id SET DEFAULT nextval('audit_seq'::regclass);


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
-- Data for Name: _pgmdd_backup_audit_2018-02-05_11:59; Type: TABLE DATA; Schema: public; Owner: epc
--

COPY "_pgmdd_backup_audit_2018-02-05_11:59" (id, vrijeme, tablica, dogadjaj, korisnik, id_tablice) FROM stdin;
29	2018-05-02 08:33:24.418595	dobavljaci	INSERT	epc	12
30	2018-05-02 08:34:13.312624	dobavljaci	INSERT	epc	12
31	2018-05-02 08:34:13.312624	dobavljaci	UPDATE	epc	12
32	2018-05-02 08:35:25.474182	dobavljaci	DELETE	epc	12
33	2018-05-02 09:15:59.165581	skladista	INSERT	epc	4
34	2018-05-02 09:16:45.764014	skladista	INSERT	epc	4
35	2018-05-02 09:16:45.764014	skladista	UPDATE	epc	4
36	2018-05-02 09:17:14.446002	skladista	DELETE	epc	4
38	2018-05-02 09:20:41.828635	supravodici	INSERT	epc	40
39	2018-05-02 09:21:29.092352	supravodici	INSERT	epc	40
40	2018-05-02 09:21:29.092352	supravodici	UPDATE	epc	40
41	2018-05-02 09:21:49.266198	supravodici	DELETE	epc	40
42	2018-05-02 09:28:00.557593	shim_zavojnice	INSERT	epc	41
43	2018-05-02 09:28:36.523287	shim_zavojnice	INSERT	epc	41
44	2018-05-02 09:28:36.523287	shim_zavojnice	UPDATE	epc	41
45	2018-05-02 09:28:59.158887	shim_zavojnice	DELETE	epc	41
46	2018-05-02 09:37:00.770106	trake	INSERT	epc	43
47	2018-05-02 09:37:41.441372	trake	INSERT	epc	42
48	2018-05-02 09:37:41.441372	trake	UPDATE	epc	42
49	2018-05-02 09:37:53.570254	trake	INSERT	epc	43
50	2018-05-02 09:37:53.570254	trake	UPDATE	epc	43
51	2018-05-02 09:38:12.483329	trake	DELETE	epc	42
52	2018-05-02 09:42:32.019321	trake	INSERT	epc	44
53	2018-05-02 09:44:35.82028	stitovi	INSERT	epc	44
54	2018-05-02 09:44:35.82028	stitovi	UPDATE	epc	44
55	2018-05-02 09:44:55.514963	stitovi	DELETE	epc	44
\.


--
-- Data for Name: audit; Type: TABLE DATA; Schema: public; Owner: epc
--

COPY audit (id, vrijeme, tablica, dogadjaj, korisnik, id_tablice, id_tablice2) FROM stdin;
1	2018-05-02 15:55:25.723329	skladista	INSERT	epc	1	\N
2	2018-05-02 15:57:34.428938	skladista	INSERT	epc	1	\N
3	2018-05-02 15:57:34.428938	skladista	UPDATE	epc	1	\N
4	2018-05-02 16:00:07.110911	dobavljaci	INSERT	epc	1	\N
5	2018-05-02 16:00:07.110911	dobavljaci	UPDATE	epc	1	\N
\.


--
-- Name: audit_seq; Type: SEQUENCE SET; Schema: public; Owner: epc
--

SELECT pg_catalog.setval('audit_seq', 5, true);


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
2	2018-02-18 21:24:10.28095	Fujikura	infinity	Leatherhead Road,Chessington,Surrey,KT9 2NY, UK	+44 (0) 20 8240 2000	\N	sales@fujikura.co.uk
1	2018-01-10 15:18:13.241298	Supercon	2018-01-10 15:22:07.552919	\N	\N	\N	\N
1	2018-01-08 04:19:37.661234	D2	2018-01-10 15:22:07.552919	Pavlinska bb, Varazdin	\N	\N	\N
1	2018-01-10 15:22:07.552919	Supercon Inc.	2018-05-02 16:00:07.110911	\N	\N	\N	\N
1	2018-05-02 16:00:07.110911	Supercon Inc.	infinity	830 Boston Turnpike, Shrewsbury, MA 01545, United States	+1 508-842-0174	(508) 842-0847	 sales@supercon-wire.com
\.


--
-- Name: dobavljaci_seq; Type: SEQUENCE SET; Schema: public; Owner: epc
--

SELECT pg_catalog.setval('dobavljaci_seq', 12, true);


--
-- Data for Name: dobavljaci_temp; Type: TABLE DATA; Schema: public; Owner: epc
--

COPY dobavljaci_temp (id, vrijeme_od, naziv, vrijeme_do, adresa, telefon, telefon2, "e-mail") FROM stdin;
1	2018-01-10 15:22:07.552919	Supercon Inc.	infinity	\N	\N	\N	\N
2	2018-02-18 21:24:10.28095	Fujikura	infinity	Leatherhead Road,Chessington,Surrey,KT9 2NY, UK	+44 (0) 20 8240 2000	\N	sales@fujikura.co.uk
\.


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
14	2018-01-10 17:43:44.69614	1	2018-01-10 15:22:07.552919	2018-05-02 07:16:50.659087	SH-Z1	Z1	25	Preko glavne zavojnice	10	10.8000002
14	2018-01-10 17:43:44.69614	1	2018-01-10 15:22:07.552919	infinity	SH-Z1	Z1	25	Preko glavne zavojnice	10	10.8000002
15	2018-01-10 17:44:21.360294	1	2018-01-10 15:22:07.552919	2018-05-02 07:16:50.659087	SH-Z1	Z2	25	Preko glavne zavojnice	10	4.73999977
15	2018-01-10 17:44:21.360294	1	2018-01-10 15:22:07.552919	infinity	SH-Z1	Z2	25	Preko glavne zavojnice	10	4.73999977
\.


--
-- Data for Name: skladista; Type: TABLE DATA; Schema: public; Owner: epc
--

COPY skladista (id, vrijeme_od, vrijeme_do, lokacija, telefon, faks, oznaka) FROM stdin;
1	2018-05-02 15:55:25.723329	2018-05-02 15:57:34.428938	Industrijska zona A, blok 3	+385 1 1112222	+385 1 1112222	Skladište 1
1	2018-05-02 15:57:34.428938	infinity	Industrijska zona A, blok 3, Dugopolje	+385 1 1112222	+385 1 1112223	Skladište 1
\.


--
-- Name: skladista_seq; Type: SEQUENCE SET; Schema: public; Owner: epc
--

SELECT pg_catalog.setval('skladista_seq', 7, true);


--
-- Data for Name: stanje; Type: TABLE DATA; Schema: public; Owner: epc
--

COPY stanje (skladiste, vrijeme_skladista, materijal, vrijeme_materijala, vrijeme_od, kolicina, vrijeme_do, biljeska, mjera) FROM stdin;
\.


--
-- Data for Name: stitovi; Type: TABLE DATA; Schema: public; Owner: epc
--

COPY stitovi (id, vrijeme_od, dobavljac, vrijeme_dobavljaca, vrijeme_do, naziv, materijal, gustoca, debljina_zida, gustoca_mag_toka, faktor_zastite, efikasnost_zastite, krit_temp_zero, krit_temp_srednje, unut_promjer, duljina) FROM stdin;
21	2018-03-01 09:52:59.254188	3	2018-03-01 09:03:49.464044	2018-03-01 09:53:10.027462	CST-10/80	Bi1.8Pb0.26Sr2Ca2Cu3O10+x (2223 phase)	\N	\N	\N	\N	\N	\N	\N	\N	\N
21	2018-03-01 09:53:10.027462	3	2018-03-01 09:03:49.464044	2018-05-02 07:16:50.691287	CST-10/80	Bi1.8Pb0.26Sr2Ca2Cu3O10+x (2223 phase)	5	1.5	5	10000000	120	108	110	10	80
21	2018-03-01 09:52:59.254188	3	2018-03-01 09:03:49.464044	2018-03-01 09:53:10.027462	CST-10/80	Bi1.8Pb0.26Sr2Ca2Cu3O10+x (2223 phase)	\N	\N	\N	\N	\N	\N	\N	\N	\N
21	2018-03-01 09:53:10.027462	3	2018-03-01 09:03:49.464044	infinity	CST-10/80	Bi1.8Pb0.26Sr2Ca2Cu3O10+x (2223 phase)	5	1.5	5	10000000	120	108	110	10	80
\.


--
-- Data for Name: supravodici; Type: TABLE DATA; Schema: public; Owner: epc
--

COPY supravodici (id, vrijeme_od, dobavljac, vrijeme_dobavljaca, vrijeme_do, naziv, tip, cisti_promjer, promjer_izolator, broj_niti, cu_sc, "kriticna_struja3T", "kriticna_struja5T", "kriticna_struja7T", "kriticna_struja9T", promjer_niti) FROM stdin;
5	2018-01-10 14:57:17.900401	1	2018-01-10 15:22:07.552919	infinity	Multifilament Wire	56S53	0.300000012	0.330000013	56	0.899999976	125	100	55	20	30
8	2018-01-10 15:02:13.791605	1	2018-01-10 15:22:07.552919	infinity	Multifilament Wire	54S43	0.300000012	0.330000013	54	1.29999995	100	80	45	16	25
10	2018-01-10 15:02:13.809029	1	2018-01-08 04:19:37.661234	2018-05-02 07:16:50.695841	Multifilament Wire	54S33	0.400000006	0.430000007	54	2	150	110	70	23	31
10	2018-01-10 15:02:13.809029	1	2018-01-10 15:22:07.552919	infinity	Multifilament Wire	54S33	0.400000006	0.430000007	54	2	150	110	70	23	31
6	2018-01-10 15:07:03.767644	1	2018-01-08 04:19:37.661234	2018-05-02 07:16:50.695841	Multifilament Wire	MR24	0.699999988	0.75	24	7	160	120	75	25	50
6	2018-01-10 15:07:03.767644	1	2018-01-10 15:22:07.552919	infinity	Multifilament Wire	MR24	0.699999988	0.75	24	7	160	120	75	25	50
7	2018-01-10 15:07:03.781099	1	2018-01-10 15:22:07.552919	infinity	Multifilament Wire	MR16	1.25	1.29999995	16	11	345	235	160	50	90
11	2018-01-10 15:12:56.372402	1	2018-01-08 04:19:37.661234	2018-05-02 07:16:50.695841	Monofilament Wire	T48B-M	0.300000012	0.330000013	1	1.5	130	90	60	20	185
11	2018-01-10 15:12:56.372402	1	2018-01-10 15:22:07.552919	infinity	Monofilament Wire	T48B-M	0.300000012	0.330000013	1	1.5	130	90	60	20	185
12	2018-01-10 15:15:33.903181	1	2018-01-08 04:19:37.661234	2018-05-02 07:16:50.695841	Monofilament Wire	T48B-G	0.400000006	0.430000007	1	3	110	80	50	18	200
12	2018-01-10 15:15:33.903181	1	2018-01-10 15:22:07.552919	infinity	Monofilament Wire	T48B-G	0.400000006	0.430000007	1	3	110	80	50	18	200
13	2018-01-10 15:15:33.914603	1	2018-01-08 04:19:37.661234	2018-05-02 07:16:50.695841	Monofilament Wire	T48B-G	0.699999988	0.753000021	1	3	250	180	110	45	350
13	2018-01-10 15:15:33.914603	1	2018-01-10 15:22:07.552919	infinity	Monofilament Wire	T48B-G	0.699999988	0.753000021	1	3	250	180	110	45	350
\.


--
-- Data for Name: trake; Type: TABLE DATA; Schema: public; Owner: epc
--

COPY trake (id, vrijeme_od, dobavljac, vrijeme_dobavljaca, vrijeme_do, naziv, sirina, debljina, supstrat, stabilizator, krit_struja) FROM stdin;
17	2018-02-18 21:38:59.641284	2	2018-02-18 21:24:10.28095	2018-05-02 07:16:50.706195	FYSC-SC05	5	0.159999996	75	75	250
17	2018-02-18 21:38:59.641284	2	2018-02-18 21:24:10.28095	infinity	FYSC-SC05	5	0.159999996	75	75	250
18	2018-02-18 21:39:41.153549	2	2018-02-18 21:24:10.28095	2018-05-02 07:16:50.706195	FYSC-SC10	10	0.159999996	75	75	500
18	2018-02-18 21:39:41.153549	2	2018-02-18 21:24:10.28095	infinity	FYSC-SC10	10	0.159999996	75	75	500
19	2018-02-18 21:40:23.537238	2	2018-02-18 21:24:10.28095	2018-05-02 07:16:50.706195	FYSC-S05	5	0.0799999982	75	0	250
19	2018-02-18 21:40:23.537238	2	2018-02-18 21:24:10.28095	infinity	FYSC-S05	5	0.0799999982	75	0	250
20	2018-02-18 21:40:48.152601	2	2018-02-18 21:24:10.28095	2018-05-02 07:16:50.706195	FYSC-S10	10	0.0799999982	75	0	500
20	2018-02-18 21:40:48.152601	2	2018-02-18 21:24:10.28095	infinity	FYSC-S10	10	0.0799999982	75	0	500
\.


--
-- Name: audit_pkey; Type: CONSTRAINT; Schema: public; Owner: epc
--

ALTER TABLE ONLY audit
    ADD CONSTRAINT audit_pkey PRIMARY KEY (id);


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

