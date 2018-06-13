using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace GUIFrontEnd
{
    class StatOptimize
    {
        private static string[] names = {
            "Poise",

            "Physical",
            "Strike",
            "Slash",
            "Thrust",

            "Magic",
            "Fire",
            "Dark",
            "Lightning",

            "Poison",
            "Bleed",
            "Petrifry",
            "Curse",

            "Lowest Element"
        };

        StatOptimize(UInt16 _Data) { Data = _Data; } 
        public UInt16 Data { get; set; }

        public override string ToString()
        {
            return names[Data];
        }


        public static StatOptimize[] Optimizer =
        {
            new StatOptimize(0),
            new StatOptimize(1),
            new StatOptimize(2),
            new StatOptimize(3),
            new StatOptimize(4),
            new StatOptimize(5),
            new StatOptimize(6),
            new StatOptimize(7),
            new StatOptimize(8),
            new StatOptimize(9),
            new StatOptimize(10),
            new StatOptimize(11),
            new StatOptimize(12),
            new StatOptimize(13)
        };
    }



}
