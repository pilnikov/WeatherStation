let nIntervId;

			function loadValues()
			{
				var xh = new XMLHttpRequest();
				xh.open("GET", "/jwifi", true);
				xh.send(null);
				xh.onreadystatechange =  function()
				{
					if (this.readyState == 4 && this.status == 200) 
					{
						var res = JSON.parse(xh.responseText);
						document.getElementById('aps' ).value = res.apid;
						document.getElementById('app' ).value = res.appas;
						document.getElementById('stas').value = res.staid;
						document.getElementById('stap').value = res.stapas;
						document.getElementById('pin1').value = res.pin1_name;
						document.getElementById('pin2').value = res.pin2_name;
						document.getElementById('on1_code').value = res.on1_code;
						document.getElementById('off1_code').value = res.off1_code;
						document.getElementById('on2_code').value = res.on2_code;
						document.getElementById('off2_code').value = res.off2_code;
						document.getElementById('lim_l').value = res.lim_l;
						document.getElementById('lim_h').value = res.lim_h;
					}
				};
			}

			function Q1_On_Off()
			{
				var xh = new XMLHttpRequest();
				xh.open('GET', '/q1_set', true);
				xh.send(null);
			}

			function Q1_Auto()
			{
				var xh = new XMLHttpRequest();
				xh.open('GET', '/q1_auto', true);
				xh.send(null);
			}

			function Q2_On_Off()
			{
				var xh = new XMLHttpRequest();
				xh.open('GET', '/q2_set', true);
				xh.send(null);
			}

			function Q2_Auto()
			{
				var xh = new XMLHttpRequest();
				xh.open('GET', '/q2_auto', true);
				xh.send(null);
			}

			function set_wifi()
			{
				var as  = document.getElementById('aps').value;
				var ap  = document.getElementById('app').value;
				var ss  = document.getElementById('stas').value;
				var sp  = document.getElementById('stap').value;

				var p1  = document.getElementById('pin1').value;
				var p2  = document.getElementById('pin2').value;
				var on1 = document.getElementById('on1_code').value;
				var of1 = document.getElementById('off1_code').value;
				var on2 = document.getElementById('on2_code').value;
				var of2 = document.getElementById('off2_code').value;
				var ll  = document.getElementById('lim_l').value;
				var lh  = document.getElementById('lim_h').value;

				var url2 = {
                        as: as,
                        ap: ap,
                        ss: ss,
                        sp: sp,

                        p1: p1,
                        p2: p2,
                        on1: on1,
                        of1: of1,
                        on2: on2,
                        of2: of2,
                        ll: ll,
                        lh: lh
                      };

				var url2Str = '/set_wifi?in=' + JSON.stringify(url2);
			
			  send_url(url2Str);
			}
			
			function send_url(url)
			{
				var xh = new XMLHttpRequest();
				xh.open('GET', url, true);
				xh.send(null);
				xh.onreadystatechange = function()
				{
					if (this.readyState == 4 && this.status == 200) loadValues();
				};
			}
		  
			function process()
			{
				var xh = new XMLHttpRequest();
				xh.open("GET", "/jwifi", true);
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
					}
				};
			}
//------------------------------------------------------------------------------Common
			function showHide(element_id, visible) 
			{
				if (document.getElementById(element_id)) //Если элемент с id-шником element_id существует 
				{ 
					var obj = document.getElementById(element_id); //Записываем ссылку на элемент в переменную obj
					if (visible) obj.style.display = "inline-block"; //Показываем элемент
					else obj.style.display = "none"; //Скрываем элемент
				}
				else alert("Элемент с id: " + element_id + " не найден!"); //Если элемент с id-шником element_id не найден, то выводим сообщение
			}   

      nIntervId = setInterval(process, 500);