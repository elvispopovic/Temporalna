CREATE OR REPLACE FUNCTION azuriranje_stitova() RETURNS TRIGGER AS
$$
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
$$LANGUAGE plpgsql;

DROP TRIGGER IF EXISTS stitovi_okidac ON stitovi;
CREATE TRIGGER stitovi_okidac BEFORE INSERT OR DELETE OR UPDATE ON stitovi
FOR EACH ROW
EXECUTE PROCEDURE  azuriranje_stitova();

