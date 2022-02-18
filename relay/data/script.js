			let nIntervId;
			let req_done = true;

			function loadVal_WiFi()
			{
				let url = '/jwifi';

				if (req_done)
				{
					rec_done = false;
					var xh = new XMLHttpRequest();
					xh.open('GET', url, true);
					xh.send(null);
					xh.onreadystatechange = function()
					{
						if (this.readyState == 4 && this.status == 200) 
						{
							var res = JSON.parse(xh.responseText);
							document.getElementById('aps' ).value   = res.as;
							document.getElementById('app' ).value   = res.ap;
							document.getElementById('stas').value  = res.ss1;
							document.getElementById('stap').value  = res.sp1;
							rec_done = true;
							loadVal_Par();
						}
					};
				}
			}
	  
			function loadVal_Par()
			{
				let url = '/jpar';

				if (req_done)
				{
					rec_done = false;
					var xh = new XMLHttpRequest();
					xh.open('GET', url, true);
					xh.send(null);
					xh.onreadystatechange = function()
					{
						if (this.readyState == 4 && this.status == 200) 
						{
							var res = JSON.parse(xh.responseText);
							document.getElementById('pin1').value = res.pin1_name;
							document.getElementById('pin2').value = res.pin2_name;
							document.getElementById('on1_code').value = res.on1_code;
							document.getElementById('off1_code').value = res.off1_code;
							document.getElementById('on2_code').value = res.on2_code;
							document.getElementById('off2_code').value = res.off2_code;
							document.getElementById('lim_l').value = res.lim_l;
							document.getElementById('lim_h').value = res.lim_h;
							rec_done = true;
						}
					};
				}
			}
							
			function Q1_On_Off()
			{
				let url = '/q1_set';

				if (req_done)
				{
					rec_done = false;
					var xh = new XMLHttpRequest();
					xh.open('GET', url, true);
					xh.send(null);
					xh.onreadystatechange = function()
					{
						if (this.readyState == 4 && this.status == 200) 
						{
							rec_done = true;
						}
					};
				}
			}

			function Q1_Auto()
			{
				let url = '/q1_auto';

				if (req_done)
				{
					rec_done = false;
					var xh = new XMLHttpRequest();
					xh.open('GET', url, true);
					xh.send(null);
					xh.onreadystatechange = function()
					{
						if (this.readyState == 4 && this.status == 200) 
						{
							rec_done = true;
						}
					};
				}
			}

			function Q2_On_Off()
			{
				let url = '/q2_set';

				if (req_done)
				{
					rec_done = false;
					var xh = new XMLHttpRequest();
					xh.open('GET', url, true);
					xh.send(null);
					xh.onreadystatechange = function()
					{
						if (this.readyState == 4 && this.status == 200) 
						{
							rec_done = true;
						}
					};
				}
			}

			function Q2_Auto()
			{
				let url = '/q2_auto';

				if (req_done)
				{
					rec_done = false;
					var xh = new XMLHttpRequest();
					xh.open('GET', url, true);
					xh.send(null);
					xh.onreadystatechange = function()
					{
						if (this.readyState == 4 && this.status == 200) 
						{
							rec_done = true;
						}
					};
				}
			}

			function set_wifi()
			{
				var as = document.getElementById('aps').value;
				var ap = document.getElementById('app').value;
				var ss1 = document.getElementById('stas').value;
				var sp1 = document.getElementById('stap').value;
				var ss2 = 'none';
				var sp2 = 'empty';

				var st1 = 0;
				var st2 = 0;

				var iap = '192.168.4.1';
				var map = '255.255.255.0';

				var wof = 0;

				let url2  = {
							  as: as,
							  ap: ap,
							 iap: iap,
							 map: map,
							 ss1: ss1,
							 sp1: sp1,
							 st1: st1,
							 ss2: ss2,
							 sp2: sp2,
							 st2: st2,
							 wof: wof
							};

				var url2Str = '/set_wifi?in=' + JSON.stringify(url2);
			
			  send_url(url2Str);
			}

			function set_par()
			{
				var p1  = document.getElementById('pin1').value;
				var p2  = document.getElementById('pin2').value;
				var on1 = document.getElementById('on1_code').value;
				var of1 = document.getElementById('off1_code').value;
				var on2 = document.getElementById('on2_code').value;
				var of2 = document.getElementById('off2_code').value;
				var ll  = document.getElementById('lim_l').value;
				var lh  = document.getElementById('lim_h').value;

				var url2 = {
                        p1: p1,
                        p2: p2,
                        on1: on1,
                        of1: of1,
                        on2: on2,
                        of2: of2,
                        ll: ll,
                        lh: lh
                      };

				var url2Str = '/set_par?in=' + JSON.stringify(url2);
			
			  send_url(url2Str);
			}
			
			function send_url(url)
			{
				if (req_done)
				{
					rec_done = false;
					var xh = new XMLHttpRequest();
					xh.open('GET', url, true);
					xh.send(null);
					xh.onreadystatechange = function()
					{
						if (this.readyState == 4 && this.status == 200) 
						{
					    loadVal_WiFi();
							rec_done = true;
						}
					};
				}
			}
			
			function process()
			{
				let url = '/jact';

				if (req_done)
				{
					rec_done = false;
					var xh = new XMLHttpRequest();
					xh.open('GET', url, true);
					xh.send(null);
					xh.onreadystatechange = function()
					{
						if (this.readyState == 4 && this.status == 200) 
						{
						
							var res = JSON.parse(xh.responseText);

  						document.getElementById('cur_br').value = res.ana_code;
  						
  						if ( res.out1_state)  document.getElementById(  'on1').value = 'On';
  						if (!res.out1_state)  document.getElementById(  'on1').value = 'Off';
  						if ( res.out1_auto)   document.getElementById('auto1').value = 'Auto';
  						if (!res.out1_auto)   document.getElementById('auto1').value = 'Manual';
  					
  						if ( res.out2_state)  document.getElementById(  'on2').value = 'On';
  						if (!res.out2_state)  document.getElementById(  'on2').value = 'Off';
  						if ( res.out2_auto)   document.getElementById('auto2').value = 'Auto';
  						if (!res.out2_auto)   document.getElementById('auto2').value = 'Manual';
  
  						var d1 = document.getElementById('lim_h').value;
  						document.getElementById('rlimh').value = d1;
  
  						d1 = document.getElementById('lim_l').value;
  						document.getElementById('rliml').value = d1;
    					
    					rec_done = true;
  					}
  				};
  			}
  		}
//------------------------------------------------------------------------------Common

      nIntervId = setInterval(process, 600);