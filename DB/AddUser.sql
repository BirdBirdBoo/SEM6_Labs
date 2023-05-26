CREATE LOGIN [pavlo-ivan.mykhalevych.pz.2020@edu.lpnu.ua] FROM EXTERNAL PROVIDER

CREATE USER [pavlo-ivan.mykhalevych.pz.2020@edu.lpnu.ua] 
FOR LOGIN [pavlo-ivan.mykhalevych.pz.2020@edu.lpnu.ua] 
WITH DEFAULT_SCHEMA = dbo; 

ALTER ROLE db_datareader ADD MEMBER [pavlo-ivan.mykhalevych.pz.2020@edu.lpnu.ua]; 
ALTER ROLE db_datawriter ADD MEMBER [pavlo-ivan.mykhalevych.pz.2020@edu.lpnu.ua]; 
ALTER ROLE db_ddladmin ADD MEMBER [pavlo-ivan.mykhalevych.pz.2020@edu.lpnu.ua]; 

GRANT VIEW DATABASE STATE TO [pavlo-ivan.mykhalevych.pz.2020@edu.lpnu.ua]