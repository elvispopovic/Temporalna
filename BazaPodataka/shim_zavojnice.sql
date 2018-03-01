CREATE OR REPLACE FUNCTION public.azuriranje_shim_zavojnice()
 RETURNS trigger
 LANGUAGE plpgsql
AS $function$
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
$function$
