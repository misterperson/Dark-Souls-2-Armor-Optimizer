using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.IO;
using Newtonsoft.Json;

namespace GUIFrontEnd
{
    public class Stats
    {
        public UInt16 Strength { get; set; }
        public UInt16 Dexterity { get; set; }
        public UInt16 Intelligence { get; set; }
        public UInt16 Faith { get; set; }

        public UInt16 Poise { get; set; }

        public UInt16 Physical { get; set; }
        public UInt16 Strike { get; set; }
        public UInt16 Slash { get; set; }
        public UInt16 Thrust { get; set; }

        public UInt16 Magic { get; set; }
        public UInt16 Fire { get; set; }
        public UInt16 Dark { get; set; }
        public UInt16 Lightning { get; set; }

        public UInt16 Poison { get; set; }
        public UInt16 Bleed { get; set; }
        public UInt16 Petrification { get; set; }
        public UInt16 Curse { get; set; }
    }

    public class ForcedEq
    {
        public UInt32[] Head { get; set; }
        public UInt32[] Body {get; set;}
        public UInt32[] Arms {get; set;}
        public UInt32[] Legs {get; set;}
    }

    public class Constraints
    {
        public UInt16[] Poise { get; set; }

        public UInt16[] Physical { get; set; }
        public UInt16[] Strike { get; set; }
        public UInt16[] Slash { get; set; }
        public UInt16[] Thrust { get; set; }

        public UInt16[] Magic { get; set; }
        public UInt16[] Fire { get; set; }
        public UInt16[] Dark { get; set; }
        public UInt16[] Lightning { get; set; }

        public UInt16[] Poison { get; set; }
        public UInt16[] Bleed { get; set; }
        public UInt16[] Petrification { get; set; }
        public UInt16[] Curse { get; set; }
    }



    public class Settings
    {
        public Double Max_Burden { get; set; }
        public Double Weight_Without_Armor { get; set; }
        public Double Target_Equipload { get; set; }

        public Stats Stats { get; set; }

        public ForcedEq Forced_Equipment {get; set; }

        public Constraints Constraints { get; set; }

        public UInt16 Optimize_For { get; set; }
    }

    class Writer
    {
        public Settings settings { get; set; }

        public Writer(Settings _settings) { settings = _settings; }

        public void WriteOut()
        {
            string json = JsonConvert.SerializeObject(settings, Formatting.Indented);
            json = json.Replace('_', ' ');
            using (StreamWriter writer = new StreamWriter("Input.json"))
            {
                writer.Write(json);
            }
        }

    }
}
