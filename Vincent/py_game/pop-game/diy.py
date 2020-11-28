import json

def strSwitch(s):
    """
        Change string to some correct values.
    """
    if s == "True":
        return True
    elif s == "False":
        return False
    else:
        s = s.replace("(", "").replace(")", "")
        values = s.split(",")
        values = [int(i) for i in values]
        return tuple(values)

def check_color(c_tuple):
    """
        to ensure the color tuple is in the range
    """
    for i in range(len(c_tuple)):
        if c_tuple[i]>255:
            c_tuple[i] = 255
        elif c_tuple[i]<0:
            c_tuple[i] = 0
    return c_tuple

with open("config.json", "r") as f:
    conf = json.load(f)
    block_color = strSwitch(conf["animals"]["block_color"]) 
    block_color = check_color(block_color)
    skip = strSwitch(conf["animals"]["skip"])
    col_margin = conf["animals"]["col_margin"]
    row_margin = conf["animals"]["row_margin"]
    result_bg_color = strSwitch(conf["app"]["result"]["bg_color"])
    result_bg_color = check_color(result_bg_color)
    result_font_color = strSwitch(conf["app"]["result"]["font_color"])
    result_font_color = check_color(result_font_color)
    result_font_angle = conf["app"]["result"]["font_angle"]
    img_size = conf["config"]["img_size"]
    screen_size = conf["config"]["screen_size"]
    col_num = conf["config"]["col_num"]
    total_num = conf["config"]["total_num"]
    margin = conf["config"]["margin"]