using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Diagnostics;
using System.Drawing;
using System.IO;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;

namespace GUIFrontEnd
{
    public partial class ScholarArmorOptimizer : Form
    {
        public static ArmorSettings armorSettings { get; set; } 
        public static Loader loader = new Loader();
        string outputBuf = "";
        public ScholarArmorOptimizer()
        {
            InitializeComponent();
        }

        public void resetArmor()
        {
            for (int i = 0; i != armorSettings.head.Length; ++i)
                armorSettings.head[i] = true;
            for (int i = 0; i != armorSettings.body.Length; ++i)
                armorSettings.body[i] = true;
            for (int i = 0; i != armorSettings.arms.Length; ++i)
                armorSettings.arms[i] = true;
            for (int i = 0; i != armorSettings.legs.Length; ++i)
                armorSettings.legs[i] = true;
        }

        private void Form1_Load(object sender, EventArgs e)
        {


            HeadRestriction.DataSource = loader.head.Gear;
            HeadRestriction.SelectedIndexChanged += HeadRestriction_SelectedIndexChanged;

            BodyRestriction.DataSource = loader.body.Gear;

            ArmRestriction.DataSource = loader.arms.Gear;

            LegRestriction.DataSource = loader.legs.Gear;

            OptimizeFor.DataSource = StatOptimize.Optimizer;

            MaxEquipLoad.KeyPress += Decimal_KeyPress;
            GearWeight.KeyPress += Decimal_KeyPress;
            RollBox.KeyPress += Decimal_KeyPress;
            RollBox.Validating += RollBox_TextChanged;


            armorSettings = new ArmorSettings()
            {
                head = new bool[loader.head.Gear.Count - 1],
                body = new bool[loader.body.Gear.Count - 1],
                arms = new bool[loader.arms.Gear.Count - 1],
                legs = new bool[loader.legs.Gear.Count - 1]
            };

            resetArmor();
        }

        private void RollBox_TextChanged(object sender, CancelEventArgs e)
        {
        }

        private void Decimal_KeyPress(object sender, KeyPressEventArgs e)
        {
            if (!char.IsControl(e.KeyChar) && !char.IsDigit(e.KeyChar) &&
                (e.KeyChar != '.'))
            {
                e.Handled = true;
            }

            // only allow one decimal point
            if ((e.KeyChar == '.') && ((sender as TextBox).Text.IndexOf('.') > -1))
            {
                e.Handled = true;
            }
        }

        private void HeadRestriction_SelectedIndexChanged(object sender, EventArgs e)
        {
            Armor foo = (Armor)HeadRestriction.SelectedItem;
        }

        // why does the designer keep making random functions for me
        private void label1_Click(object sender, EventArgs e)
        {

        }

        private void label3_Click(object sender, EventArgs e)
        {

        }

        private void textBox1_TextChanged(object sender, EventArgs e)
        {

        }

        private void numericUpDown1_ValueChanged(object sender, EventArgs e)
        {
        }
       
        public delegate void Textupdater();
        private void button1_Click(object sender, EventArgs e)
        {
            StatOptimize stat = (StatOptimize)OptimizeFor.SelectedValue;

            Armor head = (Armor)HeadRestriction.SelectedValue;
            Armor body = (Armor)BodyRestriction.SelectedValue;
            Armor arms = (Armor)ArmRestriction.SelectedValue;
            Armor legs = (Armor)LegRestriction.SelectedValue;

            // holy mother of lines of code D:
            Writer writer = new Writer(new Settings()
            {
                Max_Burden = Double.Parse(MaxEquipLoad.Text),
                Weight_Without_Armor = Double.Parse(GearWeight.Text),
                Target_Equipload = Double.Parse(RollBox.Text) / 100.0,

                Stats = new Stats()
                {
                    Strength = (UInt16)Strength.Value,
                    Dexterity = (UInt16)Dexterity.Value,
                    Intelligence = (UInt16)Intelligence.Value,
                    Faith = (UInt16)Faith.Value,

                    Poise = 0,

                    Physical = (UInt16)PhysDef.Value,
                    Strike = (UInt16)PhysDef.Value,
                    Slash = (UInt16)PhysDef.Value,
                    Thrust = (UInt16)PhysDef.Value,

                    Magic = (UInt16)MagicDef.Value,
                    Fire = (UInt16)FireDef.Value,
                    Lightning = (UInt16)LightDef.Value,
                    Dark = (UInt16)DarkDef.Value,

                    Poison = 0,
                    Bleed = 0,
                    Petrification = 0,
                    Curse = 0,
                },

                Forced_Equipment = new ForcedEq()
                {
                    Head = new UInt32[0],
                    Body = new UInt32[0],
                    Arms = new UInt32[0],
                    Legs = new UInt32[0],
                },



                Constraints = new Constraints
                {
                    Poise = new UInt16[] { (UInt16)DesPoise.Value, 4096 },

                    Physical = new UInt16[] { (UInt16)DesPhys.Value, 4096 } ,
                    Strike = new UInt16[] { (UInt16)DesPhys.Value, 4096 },
                    Slash = new UInt16[] { (UInt16)DesPhys.Value, 4096 },
                    Thrust = new UInt16[] { (UInt16)DesPhys.Value, 4096 },

                    Magic = new UInt16[] { (UInt16)DesMag.Value, 4096 },
                    Fire = new UInt16[] { (UInt16)DesFire.Value, 4096 },
                    Lightning = new UInt16[] { (UInt16)DesLight.Value, 4096 },
                    Dark = new UInt16[] { (UInt16)DesDark.Value, 4096 },

                    Bleed = new UInt16[] { 0, 4096 },
                    Poison = new UInt16[] { 0, 4096 },
                    Petrification = new UInt16[] { 0, 4096 },
                    Curse = new UInt16[] { 0, 4096 }
                },
                Optimize_For = stat.Data


            });

            if (head.ItemID != Armor.NoArmor)
            {
                writer.settings.Forced_Equipment.Head = new UInt32[] { head.ItemID };
            }
            else
            {
                List<UInt32> allowed = new List<UInt32>();
                int i = 0;
                foreach (var gear in armorSettings.head)
                {
                    if (gear) allowed.Insert(0, loader.head.Gear[i + 1].ItemID);
                    i++;
                }

                if (allowed.Count == 0)
                {
                    allowed.Insert(0, 9999);
                }
                writer.settings.Forced_Equipment.Head = allowed.ToArray();
            }

            if (body.ItemID != Armor.NoArmor)
            {
                writer.settings.Forced_Equipment.Body = new UInt32[] { body.ItemID };
            }
            else
            {
                List<UInt32> allowed = new List<UInt32>();
                int i = 0;
                foreach (var gear in armorSettings.body)
                {
                    if (gear) allowed.Insert(0, loader.body.Gear[i + 1].ItemID);
                    i++;
                }

                if (allowed.Count == 0)
                {
                    allowed.Insert(0, 9999);
                }
                writer.settings.Forced_Equipment.Body = allowed.ToArray();
            }

            if (arms.ItemID != Armor.NoArmor)
            {
                writer.settings.Forced_Equipment.Arms = new UInt32[] { arms.ItemID };
            }
            else
            {
                List<UInt32> allowed = new List<UInt32>();
                int i = 0;
                foreach (var gear in armorSettings.arms)
                {
                    if (gear) allowed.Insert(0, loader.arms.Gear[i + 1].ItemID);
                    i++;
                }

                if (allowed.Count == 0)
                {
                    allowed.Insert(0, 9999);
                }
                writer.settings.Forced_Equipment.Arms = allowed.ToArray();
            }

            if (legs.ItemID != Armor.NoArmor)
            {
                writer.settings.Forced_Equipment.Legs = new UInt32[] { legs.ItemID };
            }
            else
            {
                List<UInt32> allowed = new List<UInt32>();
                int i = 0;
                foreach (var gear in armorSettings.legs)
                {
                    if (gear) allowed.Insert(0, loader.legs.Gear[i + 1].ItemID);
                    i++;
                }

                if(allowed.Count == 0)
                {
                    allowed.Insert(0, 9999);
                }
                writer.settings.Forced_Equipment.Legs = allowed.ToArray();
            }

            writer.WriteOut();


            // todo, create input file
            OutputBox.Text = "";

            Task T = new Task(() =>
           {
               using (Process p = new Process())
               {
                   UseWaitCursor = true;
                   ProcessStartInfo info = new ProcessStartInfo()
                   {
                       Arguments = "-terse",
                       FileName = "ArmorOptimizer.exe",
                       UseShellExecute = false,
                       RedirectStandardOutput = true,
                       RedirectStandardError = true,
                       CreateNoWindow = true,
                       WorkingDirectory = Directory.GetCurrentDirectory()
                   };
                   
                   p.StartInfo = info;
                   p.OutputDataReceived += new DataReceivedEventHandler(P_OutputDataReceived);
                   p.ErrorDataReceived += new DataReceivedEventHandler(P_ErrorDataReceived);
                   p.Start();
                   p.BeginOutputReadLine();
                   p.BeginErrorReadLine();

                   // do the things


                   p.WaitForExit();
                  
                   UseWaitCursor = false;
                   if (OutputBox.InvokeRequired)
                   {
                       OutputBox.Invoke(new Textupdater(updateTextBox));
                   }
                   else
                   {
                       OutputBox.Text = outputBuf;
                   }
                   outputBuf = "";
               }
           });
            T.Start();
            

           


        }

        private void P_ErrorDataReceived(object sender, DataReceivedEventArgs e)
        {
        }

        void updateTextBox()
{
    OutputBox.Text = outputBuf;
}

        private void P_OutputDataReceived(object sender, DataReceivedEventArgs e)
        {
            outputBuf += e.Data;
            outputBuf += Environment.NewLine;
        }

        private void DarkDesired_ValueChanged(object sender, EventArgs e)
        {

        }

        private void RollBox_TextChanged(object sender, EventArgs e)
        {

        }

        private void OutputBox_TextChanged(object sender, EventArgs e)
        {

        }

        private void ArmorFilter_Click(object sender, EventArgs e)
        {
            ArmorFiltering form = new ArmorFiltering();
            form.ShowDialog();
        }

        private void label9_Click(object sender, EventArgs e)
        {

        }

        private void label10_Click(object sender, EventArgs e)
        {

        }

        private void label11_Click(object sender, EventArgs e)
        {

        }

        private void label7_Click(object sender, EventArgs e)
        {

        }

        private void label6_Click(object sender, EventArgs e)
        {

        }

        private void label5_Click(object sender, EventArgs e)
        {

        }

        private void label4_Click(object sender, EventArgs e)
        {

        }

        private void label2_Click(object sender, EventArgs e)
        {

        }

        private void panel2_Paint(object sender, PaintEventArgs e)
        {

        }

        private void label19_Click(object sender, EventArgs e)
        {

        }

        private void label13_Click(object sender, EventArgs e)
        {

        }

        private void label14_Click(object sender, EventArgs e)
        {

        }

        private void label15_Click(object sender, EventArgs e)
        {

        }

        private void label16_Click(object sender, EventArgs e)
        {

        }

        private void label17_Click(object sender, EventArgs e)
        {

        }

        private void label18_Click(object sender, EventArgs e)
        {

        }

        private void label8_Click(object sender, EventArgs e)
        {

        }
    }
}
