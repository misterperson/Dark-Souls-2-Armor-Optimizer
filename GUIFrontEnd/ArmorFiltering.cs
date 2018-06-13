using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;

namespace GUIFrontEnd
{


    public partial class ArmorFiltering : Form
    {
        private CheckBox[] head;
        private CheckBox[] body;
        private CheckBox[] arms;
        private CheckBox[] legs;

        private ArmorSettings settings;
        public ArmorFiltering()
        {
            InitializeComponent();
        }

        // when the user clicks apply
        private void button1_Click(object sender, EventArgs e)
        {
            for (int i = 0; i < head.Length; ++i)
            {
                settings.head[i] = head[i].Checked;
            }

            for (int i = 0; i < body.Length; ++i)
            {
                settings.body[i] = body[i].Checked;
            }

            for (int i = 0; i < arms.Length; ++i)
            {
                settings.arms[i] = arms[i].Checked;
            }

            for (int i = 0; i < legs.Length; ++i)
            {
                settings.legs[i] = legs[i].Checked;
            }

            ScholarArmorOptimizer.armorSettings = settings;
            // might have to send some information back
            Close();
        }

        private void ArmorFiltering_Load(object sender, EventArgs e)
        {
            CheckPanel.AutoScroll = false;
            CheckPanel.HorizontalScroll.Enabled = false;
            CheckPanel.HorizontalScroll.Visible = false;
            CheckPanel.HorizontalScroll.Maximum = 0;
            CheckPanel.AutoScroll = true;
            // don't stomp over changes unless they are confirmed
            settings = ScholarArmorOptimizer.armorSettings;
            head = new CheckBox[settings.head.Length];
            body = new CheckBox[settings.body.Length];
            arms = new CheckBox[settings.arms.Length];
            legs = new CheckBox[settings.legs.Length];

            for (int i = 0; i < head.Length; ++i)
            {
                head[i] = new CheckBox
                {
                    Checked = settings.head[i],
                    Visible = true,
                    Parent = CheckPanel,
                    Location = new Point(16, i * 20)
                };
            }

            for (int i = 0; i < body.Length; ++i)
            {
                body[i] = new CheckBox
                {
                    Checked = settings.body[i],
                    Visible = true,
                    Parent = CheckPanel,
                    Location = new Point(16 + 200, i * 20)
                };
            }

            for (int i = 0; i < arms.Length; ++i)
            {
                arms[i] = new CheckBox
                {
                    Checked = settings.arms[i],
                    Visible = true,
                    Parent = CheckPanel,
                    Location = new Point(16 + 400, i * 20)
                };
            }

            for (int i = 0; i < legs.Length; ++i)
            {
                legs[i] = new CheckBox
                {
                    Checked = settings.legs[i],
                    Visible = true,
                    Parent = CheckPanel,
                    Location = new Point(16 + 600, i * 20)
                };
            }
        }
    }


}
