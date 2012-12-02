using System;
using System.Collections.Generic;
using System.Linq;
using System.Web;

namespace ptsi_web
{
    public class ConfigParser
    {
        private enum State { FindKey, CopyKey, FindAttributeBegin,
                             FindAttribute, CopyAttribute, FindValueAssign,
                             FindValue, CopyValue, FindAttributeEnd }
        private State state = State.FindKey;
        private Dictionary<string, Dictionary<string, string>> data = new Dictionary<string,Dictionary<string,string>>();
        private Log pLog = new Log("parser.txt");
        private string key;
        private string attribute;
        private string value;
        

        public ConfigParser()
        {
            state = State.FindKey;
        }

        public ConfigParser(string file)
        {
            state = State.FindKey;
            Parse(file);
        }

        private bool CharOkName(char c)
        {
            string set = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ_";

            foreach (char f in set)
            {
                if (f == c) return true;
            }
            return false;
        }

        private bool CharOkValue(char c)
        {
            string set = "0123456789,._abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ";

            foreach (char f in set)
            {
                if (f == c) return true;
            }
            return false;
        }



        private bool CharOkBegin(char c) { return c == '{'; }
        private bool CharOkEnd(char c) { return c == '}'; }
        private bool CharOkAssign(char c) { return c == '='; }
        private bool CharOkNewline(char c) { return c == '\n'; }

        private void Evaluate(char c)
        {
            //pLog.Write(state.ToString() + " " +c.ToString());

            switch (state)
            {
                case State.FindKey:
                    if (CharOkName(c))
                    {
                        key = "";
                        key += c;
                        state = State.CopyKey;
                    }
                    break;
                case State.CopyKey:
                    if (CharOkName(c))
                    {
                        key += c;
                    }
                    else
                    {
                        state = State.FindAttributeBegin;
                    }
                    break;
                case State.FindAttributeBegin:
                    if (CharOkBegin(c))
                    {
                        state = State.FindAttribute;
                    }
                    break;
                case State.FindAttribute:
                    if (CharOkName(c))
                    {
                        attribute = "";
                        attribute += c;
                        state = State.CopyAttribute;
                    }
                    break;
                case State.CopyAttribute:
                    if (CharOkName(c))
                    {
                        attribute += c;
                    }
                    else
                    {
                        state = State.FindValueAssign;
                    }
                    break;
                case State.FindValueAssign:
                    if (CharOkAssign(c))
                    {
                        state = State.FindValue;
                    }
                    break;
                case State.FindValue:
                    if (CharOkValue(c))
                    {
                        value = "";
                        value += c;
                        state = State.CopyValue;
                    }
                    else if (CharOkNewline(c))
                    {
                        value = "";
                        Assign(key, attribute, value);
                        state = State.FindAttributeEnd;
                    }
                    break;
                case State.CopyValue:
                    if (CharOkValue(c))
                    {
                        value += c;
                    }
                    else
                    {
                        Assign(key, attribute, value);
                        state = State.FindAttributeEnd;
                    }
                    break;
                case State.FindAttributeEnd:
                    if (CharOkEnd(c))
                    {
                        state = State.FindKey;
                    }
                    else if (CharOkName(c))
                    {
                        attribute = "";
                        attribute += c;
                        state = State.CopyAttribute;
                    }
                    break;
            }
        }


        public void Parse(string file)
        {
            pLog.Write("Parsing file " + file);

            string lines = System.IO.File.ReadAllText(file);

            foreach (char c in lines)
            {
                Evaluate(c);
            }

            pLog.Write("Parsing finished");
        }

        private void Assign(string key_, string attribute_, string value_)
        {
            pLog.Write(key_ + "." + attribute_ + " = " + value_);

            if (data.ContainsKey(key_))
            {
                if (data[key_].ContainsKey(attribute_))
                {
                    data[key_][attribute_] = value_;
                }
                else
                {
                    data[key_].Add(attribute_, value_);
                }
            }
            else
            {
                Dictionary<string, string> d = new Dictionary<string,string>();
                d.Add(attribute_, value_);
                data.Add(key_, d);
            }

        }

        public string GetValue(string key_, string attribute_)
        {
            return data[key_][attribute_];
        }



    }
}