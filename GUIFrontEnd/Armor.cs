using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace GUIFrontEnd
{

    public class Armor
    {
        public static UInt32 NoArmor = 4000;
        public static Armor UnrestrictedArmor = new Armor()
        {
            Name = "Unrestricted",
            ItemID = NoArmor
        };

        public Armor()
        {

        }

        public string Name { get; set; }
        public UInt32 ItemID { get; set; }

        public override string ToString() { return Name; }
    }

    public class Armors
    {
        public List<Armor> Gear { get; set; }
    }
}
