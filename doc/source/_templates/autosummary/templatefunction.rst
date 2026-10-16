{% set info = template_function_info(fullname) %}
{{ objname | escape | underline }}

.. currentmodule:: {{ module }}

.. py:function:: {{ objname }}[{{ info.parameters }}]{{ info.signatures[0] if info.signatures else '' }}
{% for signature in info.signatures[1:] %}                {{ objname }}[{{ info.parameters }}]{{ signature }}
{% endfor %}

   Select a specialization with ``{{ objname }}[{{ info.parameters }}](...)``.

   {% if info.deduction %}{{ info.deduction }}{% endif %}

   Template arguments:
   {% for name, kind in info.kinds %}
   * ``{{ name }}`` ({{ kind }})
   {%- endfor %}

   Supported specializations: {% for specialization in info.instantiations %}``{{ specialization }}``{% if not loop.last %}, {% endif %}{% endfor %}.

{% for overload in info.overloads %}
{% if overload.specializations %}
   For {% for specialization in overload.specializations %}``{{ specialization }}``{% if not loop.last %}, {% endif %}{% endfor %}:
{% endif %}

{% if overload.signature and (info.overloads | length > 1 or not info.signatures) %}
   .. code-block:: text

      {{ objname }}[{{ info.parameters }}]{{ overload.signature }}
{% endif %}

{{ overload.description | indent(3, true) }}
{% endfor %}
