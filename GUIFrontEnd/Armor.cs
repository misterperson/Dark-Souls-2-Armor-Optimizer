using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace GUIFrontEnd
{
    class Armor
    {
        public Armor(string _name, UInt64[] _defense, UInt16[] _statReq, UInt32 _id)
        {
            name = _name;
            defense = _defense;
            statReq = _statReq;
            id = _id;
        }

        public Armor()
        {

        }

        public string name { get; set; }
        public UInt64[] defense { get; set; }
        public UInt16[] statReq { get; set; }
        public UInt32 id { get; set; }
    }
}
