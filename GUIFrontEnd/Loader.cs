using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using Newtonsoft.Json;
using System.IO;

namespace GUIFrontEnd
{


    public class Loader
    {
        public Armors head { get; set; }
        public Armors body { get; set; }
        public Armors arms {get; set;}
        public Armors legs {get; set;}

        public Loader()
        {
            string jsonString;
            try
            {
                Console.WriteLine(Directory.GetCurrentDirectory());
                using (StreamReader reader = new StreamReader("Head.json"))
                {
                   jsonString = reader.ReadToEnd();
                }

                head = JsonConvert.DeserializeObject<Armors>(jsonString);
                head.Gear.Insert(0, Armor.UnrestrictedArmor);

                Console.WriteLine(Directory.GetCurrentDirectory());
                using (StreamReader reader = new StreamReader("Body.json"))
                {
                    jsonString = reader.ReadToEnd();
                }

                body = JsonConvert.DeserializeObject<Armors>(jsonString);
                body.Gear.Insert(0, Armor.UnrestrictedArmor);

                Console.WriteLine(Directory.GetCurrentDirectory());
                using (StreamReader reader = new StreamReader("Arms.json"))
                {
                    jsonString = reader.ReadToEnd();
                }

                arms = JsonConvert.DeserializeObject<Armors>(jsonString);
                arms.Gear.Insert(0, Armor.UnrestrictedArmor);


                Console.WriteLine(Directory.GetCurrentDirectory());
                using (StreamReader reader = new StreamReader("Legs.json"))
                {
                    jsonString = reader.ReadToEnd();
                }

                legs = JsonConvert.DeserializeObject<Armors>(jsonString);
                legs.Gear.Insert(0, Armor.UnrestrictedArmor);
            }
            catch (Exception e)
            {
                Console.WriteLine("The file could not be read:");
                Console.WriteLine(e.Message);
            }
        }
    }
}
