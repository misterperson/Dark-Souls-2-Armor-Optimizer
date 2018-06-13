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

            int width = CheckPanel.Width;
            int vertSpace = 20;
            Font font = new Font("Microsoft Sans Serif", 8.25F);
            for (int i = 0; i < head.Length; ++i)
            {
                head[i] = new CheckBox
                {
                    Checked = settings.head[i],
                    Visible = true,
                    Parent = CheckPanel,
                    Location = new Point(16, i * vertSpace),
                    Text = ScholarArmorOptimizer.loader.head.Gear[i + 1].Name,
                    Font = font,
                    Size = new Size(width/4 - 40, vertSpace - 3)
                };
            }

            for (int i = 0; i < body.Length; ++i)
            {
                body[i] = new CheckBox
                {
                    Checked = settings.body[i],
                    Visible = true,
                    Parent = CheckPanel,
                    Location = new Point(16 + width / 4, i * vertSpace),
                    Text = ScholarArmorOptimizer.loader.body.Gear[i + 1].Name,
                    Font = font,
                    Size = new Size(width/4 - 40, vertSpace - 3)
                };
            }

            for (int i = 0; i < arms.Length; ++i)
            {
                arms[i] = new CheckBox
                {
                    Checked = settings.arms[i],
                    Visible = true,
                    Parent = CheckPanel,
                    Location = new Point(16 + width / 2, i * vertSpace),
                    Text = ScholarArmorOptimizer.loader.arms.Gear[i + 1].Name,
                    Font = font,
                    Size = new Size(width/4 - 40, vertSpace - 3)
                };
            }

            for (int i = 0; i < legs.Length; ++i)
            {
                legs[i] = new CheckBox
                {
                    Checked = settings.legs[i],
                    Visible = true,
                    Parent = CheckPanel,
                    Location = new Point(16 + 3 * width / 4, i * vertSpace),
                    Text = ScholarArmorOptimizer.loader.legs.Gear[i + 1].Name,
                    Font = font,
                    Size = new Size(width/4 - 40, vertSpace - 3)
                };
            }
        }

        private void button3_Click(object sender, EventArgs e)
        {
            for (int i = 0; i < head.Length; ++i)
            {
                head[i].Checked = false;
            }
            for (int i = 0; i < body.Length; ++i)
            {
                body[i].Checked = false;
            }
            for (int i = 0; i < arms.Length; ++i)
            {
                arms[i].Checked = false;
            }
            for (int i = 0; i < legs.Length; ++i)
            {
                legs[i].Checked = false;
            }

        }

        private void button2_Click(object sender, EventArgs e)
        {
            for (int i = 0; i < head.Length; ++i)
            {
                head[i].Checked = true;
            }
            for (int i = 0; i < body.Length; ++i)
            {
                body[i].Checked = true;
            }
            for (int i = 0; i < arms.Length; ++i)
            {
                arms[i].Checked = true;
            }
            for (int i = 0; i < legs.Length; ++i)
            {
                legs[i].Checked = true;
            }
        }

        private void label1_Click(object sender, EventArgs e)
        {

        }

        private void button4_Click(object sender, EventArgs e)
        {
            // doesn't update main window
            Close();
        }
    }


}
